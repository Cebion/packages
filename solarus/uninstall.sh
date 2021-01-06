#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/ports"
PKG_NAME="solarus"

rm -rf "${INSTALLPATH}/${PKG_NAME}"
rm -f "${INSTALLPATH}/solarus.sh"

for image in system-solarus.png  system-solarus-thumb.png
do
  rm "${INSTALLPATH}/images/${image}"
done
