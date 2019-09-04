#for i in $( ls *tsv* ); do
#  cp $i C1_${i:5:5}.txt
#  touch $i C3_${i:5:5}.txt
#  touch $i C4_${i:5:5}.txt
#  touch $i C2_${i:5:5}.txt
#done

#for i in $( ls *ch2* ); do
#  cp $i C2_${i:17:5}.txt
#done 

for i in $( ls *C1*); do
  touch C2_${i:3}
done
