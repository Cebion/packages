#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius
# Coypright (C) 2020-present Cebion

INSTALLPATH="/storage/roms/ports"
PKG_NAME="blockattack"
PKG_FILE="blockattack-gamedata.zip"
PKG_VERSION="1.0"
PKG_SHASUM="67a977c7c2b9c56177d331d33b469fd2ff110ce111c4302537ea8a0ad9279fd7"
SOURCEPATH=$(pwd)
### Test and make the full path if necessary.
if [ ! -d "${INSTALLPATH}/${PKG_NAME}" ]
then
  mkdir -p "${INSTALLPATH}/${PKG_NAME}"
fi

cd ${INSTALLPATH}/${PKG_NAME}

curl -Lo ${PKG_FILE} https://github.com/Cebion/blockattack-game/releases/download/${PKG_VERSION}/${PKG_FILE}
BINSUM=$(sha256sum ${PKG_FILE} | awk '{print $1}')
if [ ! "${PKG_SHASUM}" == "${BINSUM}" ]
then
  echo "Checksum mismatch, please update the package."
  exit 1
fi

unzip -o ${PKG_FILE}
rm ${PKG_FILE}

### Create the start script
cat <<EOF >${INSTALLPATH}/"Blockattack.sh"
#!/bin/bash
source /etc/profile
/usr/bin/show_splash.sh "Block Attack -  Rise of the Blocks"
clear >/dev/console
export LD_LIBRARY_PATH=${INSTALLPATH}/${PKG_NAME}/lib:/usr/lib

cd ${INSTALLPATH}/${PKG_NAME}
  ./blockattack


EOF

### Add images if not exist folder
if [ ! -d "${INSTALLPATH}/images" ]
then
  mkdir -p "${INSTALLPATH}/images"
fi

for image in system-blockattack.png  system-blockattack-thumb.png
do
  cp "${SOURCEPATH}/${PKG_NAME}/${image}" "${INSTALLPATH}/images"
done

### Add Blockattack to the game list
if [ ! "$(grep -q 'Block Attack' ${INSTALLPATH}/gamelist.xml)" ]
then
	### Add to the game list
	xmlstarlet ed --omit-decl --inplace \
		-s '//gameList' -t elem -n 'game' \
		-s '//gameList/game[last()]' -t elem -n 'path'        -v './Blockattack.sh'\
		-s '//gameList/game[last()]' -t elem -n 'name'        -v 'Block Attack -  Rise of the Blocks'\
		-s '//gameList/game[last()]' -t elem -n 'desc'        -v 'A Tetris Attack Clone.'\
		-s '//gameList/game[last()]' -t elem -n 'genre'        -v 'Puzzle'\
		-s '//gameList/game[last()]' -t elem -n 'image'       -v './images/system-blockattack.png'\
		-s '//gameList/game[last()]' -t elem -n 'thumbnail'   -v './images/system-blockattack-thumb.png'\
                -s '//gameList/game[last()]' -t elem -n 'video'       -v './videos/system-blockattack.mp4'\
		-s '//gameList/game[last()]' -t elem -n 'rating'      -v '3.0'\
		-s '//gameList/game[last()]' -t elem -n 'releasedate' -v '2005'\
		-s '//gameList/game[last()]' -t elem -n 'developer'   -v 'Poul'\
		-s '//gameList/game[last()]' -t elem -n 'publisher'   -v 'Poul via GPL'\
		${INSTALLPATH}/gamelist.xml
fi
