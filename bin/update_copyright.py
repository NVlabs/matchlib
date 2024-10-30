#!/usr/bin/env python
#
# SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Script to update the copyright date range of all files based on the last modified date.
# The script does NOT update the following (so update them by hand if needed):
#   doc/mainpage.dox (two entries)
#   LICENSE

import os
import subprocess
import re

def main():
  repo_dir = get_git_root()
  tracked_files = get_tracked_files(repo_dir)
  for file_path in tracked_files:
    full_file_path = os.path.join(repo_dir, file_path)
    if '/doc/' not in full_file_path: # Too many special cases, skip it
      if not os.path.isdir(full_file_path):
        with open(full_file_path, 'r') as f:
          content = f.read()
          if 'NVIDIA CORPORATION' in content:
            last_year = get_last_modification_year(file_path, repo_dir)
            update_copyright_year(full_file_path, last_year, repo_dir)

def update_copyright_year(filename, last_mod_year, repo_dir):
  with open(filename, 'r') as file:
    lines = file.readlines()
  updated_lines = []
  matched = False
  for line in lines:
    match = re.search(r'Copyright \(c\) (\d{4})(-(\d{4}))?', line)
    if match and not matched: # Only replace one line per file
      matched = True
      start_year = match.group(1)
      end_year = match.group(3)
      if end_year:
        if int(last_mod_year) > int(end_year):
          new_line = re.sub(end_year, last_mod_year, line)
          updated_lines.append(new_line)
          print(f'Updating copyright of {os.path.relpath(filename,repo_dir)} to {last_mod_year}')
          continue
      elif int(last_mod_year) > int(start_year):
        new_line = line.replace(f'Copyright (c) {start_year}', f'Copyright (c) {start_year}-{last_mod_year}')
        updated_lines.append(new_line)
        print(f'Updating copyright of {os.path.relpath(filename,repo_dir)} to {last_mod_year}')
        continue
    updated_lines.append(line)
  with open(filename, 'w') as file:
      file.writelines(updated_lines)

def get_git_root():
  try:
    git_root_cmd = "git rev-parse --show-toplevel"
    result = subprocess.run(git_root_cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode == 0:
      return result.stdout.decode('utf-8').strip()
    else:
      raise Exception("Not a git repository or git command failed.")
  except Exception as e:
    print(f"Error: {e}")
    exit(1)

def get_tracked_files(repo_dir):
  """Get a list of all tracked files in the Git repository."""
  git_ls_files_cmd = "git ls-files"
  result = subprocess.run(git_ls_files_cmd.split(), cwd=repo_dir, stdout=subprocess.PIPE)
  tracked_files = result.stdout.decode('utf-8').splitlines()
  return tracked_files

def get_last_modification_year(file_path,repo_dir):
  # Get git log, ignoring the first 14 lines of the file which are the copyright header itself
  git_log_cmd = f"git log -L14,+5000:{file_path} -1 --pretty=format:%ci"
  result = subprocess.run(git_log_cmd.split(), cwd=repo_dir, stdout=subprocess.PIPE)
  commit_date = result.stdout.decode('utf-8').strip()
  return commit_date.split('-')[0]

if __name__ == "__main__":
  main()
