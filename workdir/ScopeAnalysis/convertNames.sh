for i in $( ls * ); do
  mv $i ${i:7:3}_${i:1:5}.txt
done
