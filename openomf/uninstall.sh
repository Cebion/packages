#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/ports"
PKG_NAME="openomf"

rm -rf "${INSTALLPATH}/${PKG_NAME}"
rm -f "${INSTALLPATH}/Openomf.sh"

for image in system-openomf.png  system-openomf-thumb.png
do
  rm "${INSTALLPATH}/images/${image}"
done

for video in system-openomf.mp4
do
  rm "${INSTALLPATH}/videos/${video}"
done

