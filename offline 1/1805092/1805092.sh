touch output.csv
echo student_id, score > output.csv
scores=()


if [ $# -eq 0 ]
then
    ((max_score= 100))
    ((max_student_id= 5))
else
    ((max_score= $1))
    ((max_student_id= $2))
fi

for ((id= 1805121; id< 1805121+ $max_student_id; id++))
do
    folderNotExists=1;

    for folder in $( ls Submissions/ )
    do
        if [[ $((id)) = $folder ]]
        then ((folderNotExists = 0))
        fi


    done

    if (( $folderNotExists == 1 ))
    then
        ((scores[$id] = 0))
    else
        ((scores[$id] = -1))
    fi

done



for i in $( ls Submissions/ )
do
    touch sub.txt



    #Check if folder is empty
    if ((  $(ls Submissions/$i/ | wc -l) != 1 ))
    then
        ((scores[$i] = 0))
        echo "Submission error" $i " : folder is empty"
        continue;
    else
        j=$( ls Submissions/$i/ )
    fi


    #if file name isnot id.sh
    if [[ $i != ${j:0:-3} ]]
    then
        ((scores[$i] = 0))
        echo "Submission error" $i ": incorrect file name"
        continue;

    #if file is not a sh
    elif [[ "sh" != ${j: -2} ]]
    then
        ((scores[$i] = 0))
        echo "Submission error" $i ": incorrect file, wasn't sh file"
        continue;

    fi


    #  check if the output matches

    chmod +x Submissions/$i/$j
    ./Submissions/$i/$j >sub.txt
    start='[<|>]'
    (( x = $(diff -w sub.txt AcceptedOutput.txt |grep -c $start) ))
    echo "Line differences found : $x"
    (( point = $max_score - 5 * x ))
    if(($point < 0))
    then ((point = 0))
    fi
    echo "Marks obtained: $point"
    ((scores[$i]= $point))


    #Copy checker
    for k in $( ls Submissions/ )
    do
        if ((${scores[$k]} != 0))
        then
            if [[ $i != $k ]]
            then (( var = $(diff -w Submissions/$i/*  Submissions/$k/*  | wc -l) ))

                if [ $var -eq 0 ]
                then
                    (( point = 0 - $point))
                    (( scores[$i] = $point))
                    continue;
               fi
            fi
        fi
    done

done

for (( v=1805121 ; v< 1805121+ $max_student_id; v++))
do
    #echo $v ${scores[$v]}
      echo "$v,${scores[$v]}">> output.csv
done

rm -v sub.txt
