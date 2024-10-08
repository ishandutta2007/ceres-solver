// Ceres Solver - A fast non-linear least squares minimizer
// Copyright 2023 Google Inc. All rights reserved.
// http://ceres-solver.org/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of Google Inc. nor the names of its contributors may be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Author: sameeragarwal@google.com (Sameer Agarwal)

#include "ceres/preconditioner.h"

#include "absl/log/check.h"

namespace ceres::internal {

Preconditioner::~Preconditioner() = default;

PreconditionerType Preconditioner::PreconditionerForZeroEBlocks(
    PreconditionerType preconditioner_type) {
  if (preconditioner_type == SCHUR_JACOBI ||
      preconditioner_type == CLUSTER_JACOBI ||
      preconditioner_type == CLUSTER_TRIDIAGONAL) {
    return JACOBI;
  }
  return preconditioner_type;
}

SparseMatrixPreconditionerWrapper::SparseMatrixPreconditionerWrapper(
    const SparseMatrix* matrix, const Preconditioner::Options& options)
    : matrix_(matrix), options_(options) {
  CHECK(matrix != nullptr);
}

SparseMatrixPreconditionerWrapper::~SparseMatrixPreconditionerWrapper() =
    default;

bool SparseMatrixPreconditionerWrapper::UpdateImpl(const SparseMatrix& /*A*/,
                                                   const double* /*D*/) {
  return true;
}

void SparseMatrixPreconditionerWrapper::RightMultiplyAndAccumulate(
    const double* x, double* y) const {
  matrix_->RightMultiplyAndAccumulate(
      x, y, options_.context, options_.num_threads);
}

int SparseMatrixPreconditionerWrapper::num_rows() const {
  return matrix_->num_rows();
}

}  // namespace ceres::internal
