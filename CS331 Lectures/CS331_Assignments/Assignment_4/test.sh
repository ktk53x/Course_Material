for x in {1..6}; do
	echo "Executing test_"$x"....";
	./answer < test_$x.txt;
done
