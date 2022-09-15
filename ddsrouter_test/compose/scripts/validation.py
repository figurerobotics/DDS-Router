# Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import signal
import subprocess
from enum import Enum

import log


class ReturnCode(Enum):
    """Enumeration for return codes of this script."""

    SUCCESS = 0
    TIMEOUT = 1
    HARD_TIMEOUT = 2
    DUPLICATES = 3
    NOT_VALID_MESSAGES = 4
    COMMAND_FAIL = 5
    STDERR_OUTPUT = 6


def run_command(command, timeout):
    """
    Run command with timeout.

    :param command: Command to run in list format
    :param timeout: Timeout for the process
    :return:
        - ret_code - The process exit code
        - stdout - Output of the process
        - stderr - Error output of the process
    """
    ret_code = ReturnCode.SUCCESS

    log.logger.debug(f'Running command: {command}')

    proc = subprocess.Popen(command,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            universal_newlines=True)

    try:
        proc.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        log.logger.error(
            'Timeout expired. '
            'Killing process before receiving all samples...')
        proc.send_signal(signal.SIGINT)
        ret_code = ReturnCode.TIMEOUT

    stdout, stderr = proc.communicate()

    # Check whether SIGINT was able to terminate the process
    if proc.poll() is None:
        # SIGINT couldn't terminate the process
        log.logger.error(
            'SIGINT could not kill process. '
            'Killing process hardly...')
        proc.kill()
        ret_code = ReturnCode.HARD_TIMEOUT

    if not stdout:
        stdout = ''
    if not stderr:
        stderr = ''

    return (ret_code, stdout, stderr)


def run_and_validate(
        command,
        timeout: int,
        parse_output_function,
        validate_output_function):
    """
    Run the subscriber and validate its output.

    :param command: Command to run in list format
    :param timeout: Timeout for the process
    :param parse_output_function: Function to parse the output of the process
    :param validate_output_function: Function to validate the output of process

    :return: exit code
    """
    ret_code, stdout, stderr = run_command(command, timeout)

    if ret_code != ReturnCode.SUCCESS:
        log.logger.error(
            f'Executable exited with '
            f'return code {ret_code}'
            f'\n stdout output: \n{stdout}'
            f'\n stderr output: \n{stderr}')

        return ret_code

    else:

        log.logger.debug(
            f'Executable execution output:'
            f'\n stdout output: \n{stdout}'
            f'\n stderr output: \n{stderr}')

        stdout_parsed, stderr_parsed = parse_output_function(stdout, stderr)

        validate_ret_code = validate_output_function(
            stdout_parsed,
            stderr_parsed)

        return validate_ret_code


"""
AUXILIARY GENERIC FUNCTIONS
"""


def find_duplicates(data):
    """
    Find duplicates in a list os strings.

    :param data: List of strings
    :return: List of tuples with the index of the duplicated strings
    """
    duplicates = []
    lines_seen = {}

    for idx, line in enumerate(data):
        if line not in lines_seen:
            lines_seen[line] = idx
        else:
            duplicates.append((lines_seen[line], idx))

    if duplicates:
        log.logger.info('Found duplicated messages')
    else:
        log.logger.debug('None duplicated messages found')

    return duplicates


def validate_default(stdout_parsed, stderr_parsed) -> ReturnCode:
    """Validate any data as correct."""
    if stderr_parsed != '':
        return ReturnCode.STDERR_OUTPUT
    else:
        return ReturnCode.SUCCESS


def parse_default(stdout, stderr) -> ReturnCode:
    """Return stdout and stderr as they are."""
    return (stdout, stderr)