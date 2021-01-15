#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/ports"
PKG_NAME="blockattack"

rm -rf "${INSTALLPATH}/${PKG_NAME}"
rm -f "${INSTALLPATH}/Blockattack.sh"

for image in system-blockattack.png  system-blockattack-thumb.png
do
  rm "${INSTALLPATH}/images/${image}"
done
