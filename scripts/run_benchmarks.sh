#!/bin/bash

set -e
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


cd "$SCRIPTDIR/../release"
make
cd "$SCRIPTDIR"

time ../release/bm_structure     $1 &> ../experiments/bm_structure.txt
time ../release/bm_query_runtime $1 &> ../experiments/bm_query_runtime.txt
time ../release/bm_cost_model    $1 &> ../experiments/bm_cost_model.txt
time ../release/bm_space         $1 &> ../experiments/bm_space.txt
time ../release/bm_scalability   $1 &> ../experiments/bm_scalability.txt
time ../release/bm_skew          $1 &> ../experiments/bm_skew.txt
