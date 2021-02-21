#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/roms/ports"
PKG_NAME="smw"

rm -rf "${INSTALLPATH}/${PKG_NAME}"
rm -f "${INSTALLPATH}/Smw.sh"

for image in system-smw.png  system-smw-thumb.png
do
  rm "${INSTALLPATH}/images/${image}"
done

for video in system-smw.mp4
do
  rm "${INSTALLPATH}/videos/${video}"
done

