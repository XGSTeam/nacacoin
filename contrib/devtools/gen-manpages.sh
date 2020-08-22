#!/usr/bin/env bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

NACACOIND=${BITCOIND:-$BINDIR/nacacoind}
NACACOINCLI=${BITCOINCLI:-$BINDIR/nacacoin-cli}
NACACOINTX=${BITCOINTX:-$BINDIR/nacacoin-tx}
NACACOINQT=${BITCOINQT:-$BINDIR/qt/nacacoin-qt}

[ ! -x $NACACOIND ] && echo "$NACACOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
NACAVER=($($NACACOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for bitcoind if --version-string is not set,
# but has different outcomes for bitcoin-qt and bitcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$NACACOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $NACACOIND $NACACOINCLI $NACACOINTX $NACACOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${NACAVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${NACAVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
