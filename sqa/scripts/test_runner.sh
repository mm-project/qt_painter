#!/bin/bash

exit_code=""
need_dbg=""
succ=4
GDIRNAME="expected"
ODIRNAME="output"

if [ -z "$PAINTER_EXE_NAME" ]; then
  PAINTER_EXE_NAME="painter"
fi

if [ ! -z "$PAINTER_TEST_OUT_DIRNAME" ]; then
  ODIRNAME=$PAINTER_TEST_OUT_DIRNAME
fi

testname=`basename $PWD`

export PAINTER_LOGFILE_PREFIX="painter"
#mode="regolden"
mode=""
extras=""

function extra_comparision {
    extras="$1"
}

function process_options
{
    verbose "process_options"
    unset ELEN_PAINTER_REGOLDEN
    mode=$1
    #if [ "$1" == "regolden" ]; then
    #    mode="regolden"
    #fi
}

function verbose
{
    local msg="$1"
    if [ "$need_dbg" != "" ]; then
        echo $msg
    fi
}

function prepocess
{
    if [ "$mode" = "regolden" ]; then
        export ELEN_PAINTER_REGOLDEN="1"
    fi
    
    if [ "$mode" = "debug" ]; then
        export ELEN_PAINTER_TESTDBG="1"
        export ELEN_PAINTER_STARTDBG="1"
    fi
    
    verbose "prepocess..."
    rm -rf $ODIRNAME
    mkdir -p $GDIRNAME
    mkdir $ODIRNAME
    cd $ODIRNAME
    
    cp $PAINTER_QA_DIR/etc/webrelated/* . -r
    
    if [ "$mode" != "regolden" ]; then
        cp ../$GDIRNAME/* ./ -rf
        if [ "$?" != 0 ]; then
            succ=`expr $succ - 1`
        fi
    fi
}

function postprocess
{
    
    verbose "postprocess..."
    if [ "$exit_code" != "0" ]; then
        echo "Test-failed: Abnormal exit ( code $exit_code )"
        exit 3
    else
        if [ "$mode" = "regolden" ]; then
            echo "Regoldning..."
            rm -rf ../$GDIRNAME 
            mkdir -p ../$GDIRNAME
            for e in $extras; do
                cp -rf $e ../$GDIRNAME/$e.golden &> /dev/null
            done
            cp ./logs/painter.log ../$GDIRNAME/painter.log.golden
            cp ./logs/painter.lvi ../$GDIRNAME/painter.lvi.golden
	     mv ./$testname.mp4 ../$GDIRNAME/$testname.golden.mp4
            #cp painter.out ../$GDIRNAME/painter.out.golden
            cp `find -name "*.golden*"` ../$GDIRNAME
            #cnvscprs=`find -name "*compare*"`
            #for i in $cnvscprs; do
            #    cp $i ../$GDIRNAME/$i.golden
            #done
            echo "Succesfully Regoldened"
            
            return 0;
        fi
        
        if [ -d "../$GDIRNAME" ]; then
            htmlout="DIFF.html"
            touch $htmlout
            a=`diff ./logs/painter.log painter.log.golden`
            b=`diff ./logs/painter.lvi painter.lvi.golden`
            #c=`diff painter.out painter.out.golden`
            c=""

            res_d=""
            problems=""
            extranl=""
            for ex in $extras; do
                d=`diff $ex $ex.golden`
                if [ "$d" != "" ]; then
nl="
"
                    problems="$ex <--> $ex.golden $problems $extranl"
                    $PAINTER_QA_DIR/scripts/make_diff_html.sh $ex $ex.golden
                    echo "<a href=\"$ex.html\"> $ex <--> $ex.golden</a>" >> $htmlout
                    #files[${ex}]="$ex.golden"
                fi
                res_d="$res_d$d"
                extranl=$nl
            done

        else
            succ=`expr $succ - 1`
            echo "CAN'T FIND GOLDEN DIR"
        fi
        #if [ "$a" == "" ] && [ "$b" == "" ]; then
        #    succ=3
        #else
            declare -A files
            if [ "$a" != "" ]; then
                echo "MISMTACH: logs/painter.log <--> painter.log.golden "
                files["logs/painter.log"]="painter.log.golden"
                succ=`expr $succ - 1`
                cp logs/painter.log painter.log
                $PAINTER_QA_DIR/scripts/make_diff_html.sh painter.log painter.log.golden
                echo "<a href=\"painter.log.html\">painter.log <--> painter.log.golden</a><br>" >> $htmlout

            fi
        
            if [ "$b" != "" ]; then
                succ=`expr $succ - 1`
                echo "MISMTACH: logs/painter.lvi <--> painter.lvi.golden "
                cp logs/painter.lvi painter.lvi
                $PAINTER_QA_DIR/scripts/make_diff_html.sh painter.lvi painter.lvi.golden
                echo "<a href=\"painter.lvi.html\">painter.lvi <--> painter.lvi.golden</a><br>" >> $htmlout
                files["logs/painter.lvi"]="painter.lvi.golden"
            fi

            if [ "$c" != "" ]; then
                #succ=`expr $succ - 1`
                echo "MISMTACH: painter.out <--> painter.out.golden "
            fi
IFS="
"
            if [ "$res_d" != "" ]; then
                succ=`expr $succ - 1`
                echo "MISMTACH: $problems "
                #for problem in $problems; do
                #    a=`echo $problem | sed "s/ <--> / /g"`
                #    g=`echo $a | cut -d' ' -f1`
                #    f=`echo $a | cut -d' ' -f2`
                #    $PAINTER_QA_DIR/scripts/make_diff_html.sh $f $g
                #    echo "<a href=\"$f.html\">$f <--> $g</a>" >> $htmlout
                #done

            fi

        #fi
        
        #FIXME super inefficent
        #cat painter.out | grep "#\/t" 
        #info=`cat painter.out | grep "#\/t" | awk '{print $3}'`
        #errs=`echo $info | grep ERROR`
        #fails=`echo $info | grep FAIL`
        mismatchs=`cat logs/painter.log | grep "\#e --> Error: " | grep MISMATCH | cut -d' ' -f5-6  | sed "s/ / <--> /"`
        if [ "$mismatchs" != "" ]; then 
            cat logs/painter.log | grep "\#e --> Error: " | grep MISMATCH | cut -d' ' -f5-6  | sed "s/ / <--> /" | sed "s/^/MISMATCH: /" 
            #echo "OTHER MISMATCH(s) "
            #echo $mismatchs
            #succ=`expr $succ - 1`
            #exit 0
            a=`cat logs/painter.log | grep "\#e --> Error: " | grep MISMATCH | cut -d' ' -f5-6  |sed "s/$/:/"`
            #echo "----------------$a---------------"
            IFS=":"
            for s in $a; do
                s=`echo $s | tr -d '\n'` 
                if [ "$s" != "" ]; then
                    #echo "($s)"
                    f=`echo $s | cut -d' ' -f1`
                    g=`echo $s | cut -d' ' -f2`
                    #echo "($f) ($g)"
                    $PAINTER_QA_DIR/scripts/make_diff_html.sh $f $g
                    echo "<a href=\"$f.html\">$f <--> $g</a><br>" >> $htmlout
                fi
            done

        fi
        
        if [ "$mode" = "compare" ]; then
            for k in ${!files[@]}; do
                kompare ${files[$k]} $k
            done
            
            echo "#!/bin/bash" >> other.cmpr.tmp
			cat logs/painter.log | grep "\#e --> Error: " | grep MISMATCH | cut -d' ' -f5-6  | sed "s/^/kompare /" >> other.cmpr.tmp

IFS="
"
			for problem in $problems; do
				a=`echo $problem | sed "s/ <--> / /g"`
				echo "kompare $a" >> other.cmpr.tmp
			done
			
			chmod 777 other.cmpr.tmp
			./other.cmpr.tmp

        fi
        
        if [ "$succ" != 4 ]; then
            echo "Test failed."
            exit 1
        else
            echo "Test Pass !"
            exit 0
        fi
        
        
        
        
    fi
    
}

function run 
{
    local options="$1"
    verbose "running..."
    platform=linux
    toolexe=$PAINTER_EXE_NAME
    toolpath=$PAINTER_QA_DIR/../bin/$platform
    tool=$toolpath/$toolexe
    verbose "invocation: $tool "$options" &> painter.out "

    if [ "$PAINTER_QA_XVFB" != "" ]; then
    	dsp=$(( ( RANDOM % 100000 )  + 1 ))
    	Xvfb :$dsp &> xvfb.log &
    	xvfb_pid=$!
    	sleep 1
    	export DISPLAY=:$dsp
    	ffmpeg -y -r 24 -s 1280x1024 -f x11grab -i :$dsp -codec:v libx264 $testname.mp4 &> ffmpeg.log &
	ffmpeg_pid=$!
    fi
    $tool $options &> pntr.output
    exit_code=$?
    kill -15 $ffmpeg_pid &> /dev/null
    sleep 1
    kill -9 $xvfb_pid &> /dev/null
}

function run_test
{
    local args="$1"
    
    prepocess
    run "$args"
    postprocess
}
