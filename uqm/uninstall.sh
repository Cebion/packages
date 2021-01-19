#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/ports"
PKG_NAME="uqm"

rm -rf "${INSTALLPATH}/${PKG_NAME}"
rm -f "${INSTALLPATH}/Uqm.sh"

for image in system-uqm.png  system-uqm-thumb.png
do
  rm "${INSTALLPATH}/images/${image}"
done
