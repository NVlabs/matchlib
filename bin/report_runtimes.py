#!/usr/bin/env python3

# Copyright (c) 2025, NVIDIA CORPORATION.  All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License")
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

import os
import re
import argparse
import csv
from pathlib import Path
from typing import Dict, List, Tuple, Optional

def main():
    parser = argparse.ArgumentParser(description="Parse HLS job runtimes from catapult.log files")
    parser.add_argument("--hls-dir", default="hls", help="Path to HLS directory (default: hls)")
    parser.add_argument("--csv", help="Optional path to CSV file for export")
    args = parser.parse_args()

    print(f"Searching for catapult.log files in: {args.hls_dir}")
    logfiles = find_catapult_logs(args.hls_dir)
    if not logfiles:
        print("No catapult.log files found!")
        return
    print(f"Found {len(logfiles)} catapult.log files")

    design_runtimes = {}
    for design_name, logfile_path in logfiles:
        print(f"Parsing: {design_name} ({logfile_path})")
        step_runtimes = parse_hls_runtimes(logfile_path)
        if step_runtimes:
            design_runtimes[design_name] = step_runtimes
    if not design_runtimes:
        print("No valid runtime data found!")
        return

    generate_report(design_runtimes)
    if args.csv:
        export_to_csv(design_runtimes, args.csv)

def find_catapult_logs(hls_dir: str) -> List[Tuple[str, str]]:
    """
    Find all catapult.log files in the hls/ directory recursively.
    Returns a list of tuples (design_name, logfile_path).
    """
    logfiles = []
    hls_path = Path(hls_dir)

    if not hls_path.exists():
        print(f"Warning: HLS directory '{hls_dir}' does not exist")
        return logfiles

    for logfile in hls_path.rglob("catapult.log"):
        # The directory containing the logfile is the design name
        design_name = logfile.parent.name
        logfiles.append((design_name, str(logfile)))

    return logfiles


def parse_runtime_from_line(line: str) -> Optional[float]:
    """
    Extract runtime in seconds from a log line.
    Looks for pattern like "elapsed time 15.95 seconds"
    """
    # Pattern to match "elapsed time X.XX seconds" or "elapsed time X seconds"
    pattern = r"elapsed time\s+([\d.]+)\s+seconds?"
    match = re.search(pattern, line, re.IGNORECASE)

    if match:
        try:
            return float(match.group(1))
        except ValueError:
            return None

    return None


def parse_hls_runtimes(logfile_path: str) -> Dict[str, float]:
    """
    Parse HLS runtimes from a catapult.log file.
    Returns a dictionary mapping step names to runtimes in seconds.
    """
    expected_steps = [
        "analyze", "compile", "libraries", "assembly", "loops",
        "memories", "cluster", "architect", "allocate", "schedule",
        "dpfsm", "instance", "extract"
    ]

    step_runtimes = {}

    try:
        with open(logfile_path, 'r') as f:
            for line in f:
                if "Info: Completed transformation" in line:
                    # Extract the step name and runtime
                    for step in expected_steps:
                        if step in line.lower():
                            runtime = parse_runtime_from_line(line)
                            if runtime is not None:
                                step_runtimes[step] = runtime
                            break

    except FileNotFoundError:
        print(f"Warning: Could not read logfile {logfile_path}")
        return {}
    except Exception as e:
        print(f"Warning: Error reading {logfile_path}: {e}")
        return {}

    # Check for missing steps
    missing_steps = [step for step in expected_steps if step not in step_runtimes]
    if missing_steps:
        design_name = Path(logfile_path).parent.name
        print(f"Warning: Design '{design_name}' is missing steps: {', '.join(missing_steps)}")

    return step_runtimes


def generate_report(design_runtimes: Dict[str, Dict[str, float]]) -> None:
    """
    Print a simple report with total runtime for each design.
    """
    print("\nHLS Runtime Summary:")
    print("-" * 60)

    for design_name, step_runtimes in sorted(design_runtimes.items()):
        total_runtime = sum(step_runtimes.values())
        print(f"{design_name:30} {total_runtime:8.2f} seconds")


def export_to_csv(design_runtimes: Dict[str, Dict[str, float]], csv_path: str) -> None:
    """
    Export runtime data to a CSV file.
    """
    expected_steps = [
        "analyze", "compile", "libraries", "assembly", "loops",
        "memories", "cluster", "architect", "allocate", "schedule",
        "dpfsm", "instance", "extract"
    ]

    try:
        with open(csv_path, 'w', newline='') as csvfile:
            fieldnames = ['design', 'total_runtime'] + expected_steps
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

            writer.writeheader()

            for design_name, step_runtimes in sorted(design_runtimes.items()):
                row = {'design': design_name}
                total_runtime = 0.0

                for step in expected_steps:
                    if step in step_runtimes:
                        row[step] = step_runtimes[step]
                        total_runtime += step_runtimes[step]
                    else:
                        row[step] = None  # Missing step

                row['total_runtime'] = round(total_runtime, 2)
                writer.writerow(row)

        print(f"\nCSV report exported to: {csv_path}")

    except Exception as e:
        print(f"Error writing CSV file {csv_path}: {e}")



if __name__ == "__main__":
    main()

