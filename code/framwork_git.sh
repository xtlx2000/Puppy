cd ..
rm -r ./puppy/code/*
cp -r ./framwork_lib/* ./puppy/code/
cd ./puppy/
git add -A .
git commit -m "commit"
git push
cd ../framwork_lib
