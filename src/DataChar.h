/*-------------------------------------------------------------------------------
 This file is part of Ranger.

 Ranger is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Ranger is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Ranger. If not, see <http://www.gnu.org/licenses/>.

 Written by:

 Marvin N. Wright
 Institut für Medizinische Biometrie und Statistik
 Universität zu Lübeck
 Ratzeburger Allee 160
 23562 Lübeck

 http://www.imbs-luebeck.de
 #-------------------------------------------------------------------------------*/

// Ignore in coverage report (not used in R package)
// #nocov start
#ifndef DATACHAR_H_
#define DATACHAR_H_

#include <limits.h>

#include "globals.h"
#include "Data.h"

class DataChar: public Data {
public:
  DataChar();
  DataChar(double* data_double, std::vector<std::string> variable_names, size_t num_rows, size_t num_cols, bool& error);
  virtual ~DataChar();

  double get(size_t row, size_t col) const {
    // Use permuted data for corrected impurity importance
    if (col >= num_cols) {
      col = getUnpermutedVarID(col);
      row = getPermutedSampleID(row);
    }

    if (col < num_cols_no_snp) {
      return data[col * num_rows + row];
    } else {
      // Get data out of snp storage. -1 because of GenABEL coding.
      size_t idx = (col - num_cols_no_snp) * num_rows_rounded + row;
      double result = (((snp_data[idx / 4] & mask[idx % 4]) >> offset[idx % 4]) - 1);

      // Missing value is 3
      if (result < 0 || result > 2) {
        return 3;
      } else {
        return result;
      }
    }
  }

  void reserveMemory() {
    data = new char[num_cols * num_rows];
  }

  void set(size_t col, size_t row, double value, bool& error) {
    if (value > CHAR_MAX || value < CHAR_MIN) {
      error = true;
    }
    if (floor(value) != ceil(value)) {
      error = true;
    }
    data[col * num_rows + row] = (char) value;
  }

private:
  char* data;

  DISALLOW_COPY_AND_ASSIGN(DataChar);
};

#endif /* DATACHAR_H_ */
// #nocov end
