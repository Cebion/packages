#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius
# Coypright (C) 2020-present Cebion

INSTALLPATH="/storage/roms/ports"
PKG_NAME="uqm"
PKG_FILE="uqm-gamedata.zip"
PKG_VERSION="1.0"
PKG_SHASUM="ee14c1dd9ef4bed0d9a396bd52b39cf596b81821bd73b2b6a1c93049be31b684"
SOURCEPATH=$(pwd)
### Test and make the full path if necessary.
if [ ! -d "${INSTALLPATH}/${PKG_NAME}" ]
then
  mkdir -p "${INSTALLPATH}/${PKG_NAME}"
fi

cd ${INSTALLPATH}

curl -Lo ${PKG_FILE} https://github.com/Cebion/sc2-uqm/releases/download/${PKG_VERSION}/${PKG_FILE}
BINSUM=$(sha256sum ${PKG_FILE} | awk '{print $1}')
if [ ! "${PKG_SHASUM}" == "${BINSUM}" ]
then
  echo "Checksum mismatch, please update the package."
  exit 1
fi

unzip -o ${PKG_FILE}
rm ${PKG_FILE}
rm UQM.sh

### Create the start script
cat <<EOF >${INSTALLPATH}/"Uqm.sh"
#!/bin/bash
source /etc/profile
export SDL_GAMECONTROLLERCONFIG="$(cat /storage/roms/ports/UQM/gamecontrollerdb.txt)"
/storage/roms/ports/UQM/uqm --logfile /storage/roms/ports/UQM/uqmlog.txt -x --contentdir=/storage/roms/ports/UQM/content --configdir=/storage/roms/ports/UQM/config
unset SDL_GAMECONTROLLERCONFIG

EOF

### Add images if not exist folder
if [ ! -d "${INSTALLPATH}/images" ]
then
  mkdir -p "${INSTALLPATH}/images"
fi

for image in system-uqm.png  system-uqm-thumb.png
do
  cp "${SOURCEPATH}/${image}" "${INSTALLPATH}/images"
done

### Add UQM to the game list
if [ ! "$(grep -q 'Uqm' ${INSTALLPATH}/gamelist.xml)" ]
then
	### Add to the game list
	xmlstarlet ed --omit-decl --inplace \
		-s '//gameList' -t elem -n 'game' \
		-s '//gameList/game[last()]' -t elem -n 'path'        -v './Uqm.sh'\
		-s '//gameList/game[last()]' -t elem -n 'name'        -v 'The Ur-Quan Masters'\
		-s '//gameList/game[last()]' -t elem -n 'desc'        -v 'Star Control II: The Ur-Quan Masters is the sequel to Star Control.'\
		-s '//gameList/game[last()]' -t elem -n 'image'       -v './images/system-uqm.png'\
		-s '//gameList/game[last()]' -t elem -n 'thumbnail'   -v './images/system-uqm-thumb.png'\
		-s '//gameList/game[last()]' -t elem -n 'rating'      -v '4.0'\
		-s '//gameList/game[last()]' -t elem -n 'releasedate' -v '1992'\
		-s '//gameList/game[last()]' -t elem -n 'developer'   -v 'Toys for Bob'\
		-s '//gameList/game[last()]' -t elem -n 'publisher'   -v 'Accolade'\
		${INSTALLPATH}/gamelist.xml
fi
