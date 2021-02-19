#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/roms/ports"
PKG_NAME="f2bgl"

rm -rf "${INSTALLPATH}/${PKG_NAME}"
rm -f "${INSTALLPATH}/F2bgl.sh"

for image in system-f2bgl.png  system-f2bgl-thumb.png
do
  rm "${INSTALLPATH}/images/${image}"
done

for video in system-f2bgl.mp4
do
  rm "${INSTALLPATH}/videos/${video}"
done

