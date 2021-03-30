#!/usr/bin/env sh

ROOT=/home/ljh/caffe-master
CAFFE=$ROOT/build/tools/caffe
PREFIX=$ROOT/examples/_gene_homo/sources_8000_target_4000_drcn_logscale_grid
SOLVER=$PREFIX/protos/solver.prototxt
LOGDAT=$PREFIX/log_data
GPUID=3

$CAFFE train --solver=$SOLVER --gpu $GPUID 2>&1 | tee $LOGDAT

POSTPY=$PREFIX/scripts/feature_extractor.py
DEPLOY=$PREFIX/protos/deploy.prototxt
MODELS=$PREFIX/snapshots/train_iter_10000.caffemodel
python $POSTPY $ROOT $DEPLOY $MODELS $GPUID