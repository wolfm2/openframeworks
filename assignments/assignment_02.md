### Points: 5
### Assignment
Finish modules 3-6 on SoloLearn. There will be a test next class.

Create a repo called "openframeworks"

Clone the repo to your computer.

Create a directory in the repository called  "Assignment_2"

Compile and run the example in ["Testing Your Setup"](https://openframeworks.cc/setup/xcode/)

Copy the entire directory "3DPrimitivesExample" into Assignment_2

Delete ALL contents of the directory you just copied except the contents of src and bin/data

NOTE:  There should be nothing inside of bin except the directory data

Create a README.md file at the root of Assignment_2 with the program name as a header using "#"

Create three subheaders with "##" named C++, Description and Screenshots respectively

Embed a screenshot of your finished c++ module status

Add a short description 

Add a single static screenshot and either a 5 second animated gif or an embedded youtube video of the running program

In the root of the github tree create a README.md with a short bio and a picture.

Commit and push the contents to git

Submit the url of the github dir to canvas

### Setting up Github Repositories

[Make a github account](http://github.com)

Make a repository called "openframeworks".

Clone your repository to your computer.

Add a .gitignore file to the root of your repository containing the following:
```
*~
*#
*.zip
*.tar.gz

# visual studio

*.VC.db

# osx / xcode / openframeworks

.DS_Store

DerivedData

obj
**/bin/*
!**/bin/data/
*Debug.app
```

Add, commit and push the file to the repository


## Using GIT in an OSX or Linux terminal:

```
  1. copy src and bin/data of any project to the repository
  2. cd YourNewProject		: Type "cd ", drop your new project folder in the terminal, and hit enter.
  3. git add .			: Tells git to track all project files (Repeat steps 1,2 if you add new files)
  4. git commit -am "Message"	: Tells git to create a snapshot of the project.  Use a descriptive message!
  5. git status			: Tells you what git wants to change.  Verify this before pushing!
  6. git pull			: Downloads changes from your classmates
  7. git push			: Uploads changes you've made
```

You can iterate over these steps as often as needed. Every time you get a new part of your project working you should commit it so you can return to it later if needed.

For every project, some IDE files will **never** upload.  This is intentional as all environments generate huge files which don't relate to the app functionality.  The only files you need are in the "src" and (maybe) "data" directories.

## Compiling code from the terminal

```
  1. cd TheProject		: Type "cd ", drop the project folder in the terminal, and hit enter.
  2. make			: Takes your c++ files and assembles an app.
  3. fix errors if any		: Any errors will prevent the program from compiling.
  4. make RunRelease		: Runs the program if it successfully compiled.
  - optionally -
  make clean			: Will delete *all* previously compiled objects and start fresh.
```

