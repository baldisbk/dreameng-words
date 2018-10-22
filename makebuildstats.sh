if [ $# -lt 1 ]
then
        OUTFILE=makestats.gh
else
        OUTFILE=$1
fi
if [ $# -ge 2 ]
then
        INFILE=$2
fi
if [ $# -ge 3 ]
then
        cd $3
fi

rm -f $OUTFILE
echo >>$OUTFILE "#ifndef BUILDSTATS_H"
echo >>$OUTFILE "#define BUILDSTATS_H"

if [ -n "$INFILE" ]
then
    TMP=`mktemp`
    cp $INFILE $TMP
    cat $TMP >>$OUTFILE
    rm $TMP
fi

echo >>$OUTFILE "#define BUILD_DATE \"`date -I`\""
echo >>$OUTFILE "#define BUILD_TAG \"`git log --pretty=%D | grep tag | head -n 1 | awk '{print $2}'`\""
echo >>$OUTFILE "#define BUILD_HASH \"`git log -1 --pretty=%H`\""
echo >>$OUTFILE "#endif"
echo >>$OUTFILE
