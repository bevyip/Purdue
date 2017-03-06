for file in tests/*
do
    echo "$file"
    java SimpleC < "$file"
done
