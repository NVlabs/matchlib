/*
 * Copyright (c) 2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CSV_FILE_READER__
#define __CSV_FILE_READER__

#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <boost/assert.hpp>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

/**
 * \brief A helper class to read CSV files.
 *
 * The constructor takes two arguments: a string containing the filename, and an optional string containing the delimiting character (default is ',').
 */
class CSVFileReader {
  std::string fileName;
  std::string seperator;

 public:
  CSVFileReader(std::string filename, std::string sep = ",")
      : fileName(filename), seperator(sep) {}

  std::vector<std::vector<std::string> > readCSV() {
    std::ifstream file(fileName.c_str());
    std::vector<std::vector<std::string> > dataList;
    std::string line = "";
    while (getline(file, line)) {
      std::vector<std::string> vec;
      boost::algorithm::split(vec, line, boost::is_any_of(seperator));
      dataList.push_back(vec);
    }
    file.close();
    return dataList;
  }
};

#endif
