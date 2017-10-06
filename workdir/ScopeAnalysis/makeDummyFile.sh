for i in $( ls ch3* ); do
  cp $i ch4_${i:4}
done
