#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)
# Copyright (C) 2020-present Fewtarius

# Load functions needed to send messages to the console
. /etc/profile

INSTALL_PATH="/storage/roms/gamedata/solarus"
IMAGE_PATH="/storage/roms/gamedata/images"
CFG="/storage/.emulationstation/es_systems.cfg"

if grep -q '<name>solarus</name>' "$CFG"
then
	xmlstarlet ed -L -P -d "/systemList/system[name='solarus']" $CFG
fi

if [ -f /storage/.config/emulationstation/scripts/drastic.sh ]
then
  rm -f /storage/.config/emulationstation/scripts/drastic.sh
fi


for image in system-solarus.png  system-solarus-thumb.png
do
  rm "${IMAGE_PATH}/${image}"
done

rm -rf ${INSTALL_PATH}
