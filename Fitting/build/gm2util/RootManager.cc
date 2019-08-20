

#include "TMath.h"
#include "RootManager.hh"

RootManager::RootManager(string name, TFile* file) : TNamed(name.c_str(),name.c_str()) {
  Init();
  SetFile(file);
}


//Set everything to default values
void RootManager::Init() {
  TH1::AddDirectory(kFALSE);
  rootFile = NULL;
  writeToFile = false;
}


RootManager::~RootManager() {
  this->Clear();
}


void RootManager::SetFile(TFile* file) {

  //Check we are not already writing to a file
  if(writeToFile) {
    cout << "SetFile : Already writing";
  }

  //Check ROOT file is OK
  if (!file) {
    cout << "SetFile : ROOT file ptr provided is NULL, exiting";
    exit(-1);
  }
  if (!file->IsOpen()) {
    cout << "SetFile : ROOT file provided is closed, exiting";
    exit(-1);
  }

  //Everything OK, use this ROOT file
  rootFile = file;
  writeToFile = true;
  cout<< "RootManager::SetFile : Using ROOT file " << rootFile->GetName() << endl;

}


// Write objects to file
void RootManager::WriteToFile() {

  //Check we are writing
  if(!writeToFile) {
    cout << "WriteToFile : Have not specified file";
  }

  //Check ROOT file
  if (!rootFile) {
    cout << "WriteToFile : ROOT file ptr provided is NULL, exiting";
    exit(-1);
  }
  if (!rootFile->IsOpen()) {
    cout << "WriteToFile : ROOT file provided is closed, exiting";
    exit(-1);
  }

  //Write to file
  rootFile->Write();
  cout << "RootManager::WriteToFile : ROOT file " << rootFile->GetName() << " written" << endl;
  
}


//Get a pointer to specified directory
//If "create" flag set to true, create it if it doesn't exist
TDirectory* RootManager::GetDir(TDirectory* mother, string dirRelPath, bool create) {

  //Check mother ptr
  if(!mother) {
    cout << "GetDir : ERROR: Parent directory does not exist" << endl;
    return 0;
  }

  //Try to get directory from ROOT file
  TDirectory* dirPtr = mother->GetDirectory(dirRelPath.c_str());

  //If "create" flag set, make directory if doesn't exist already
  if(dirPtr==0) {

    if(create) {

      //Check file writing has been defined, as don't want to create dir without file 
      if(!rootFile || !writeToFile) {
        cout << "GetDir : ERROR: No ROOT file defined" << endl;
        return 0;
      }

      //Create the dir and get ptr to it (don't directly use mkdir returned ptr as it points top 
      //level if provide path with hierarchy)
      mother->mkdir(dirRelPath.c_str());
      dirPtr = mother->GetDirectory(dirRelPath.c_str());
      cout << "RootManager::GetDir : Created ROOT dir " << dirPtr->GetPath() << endl;

    }
    else {
      cout << "GetDir : ERROR: Directory " << mother->GetPath() << "/" 
        << dirRelPath << " does not exist" << endl;
      return 0;
    }
   
  }

  //Change directory to this directory
  //dirPtr->cd();

  //Return pointer
  return dirPtr;

}


// Remove a directory
void RootManager::RemoveDir(TDirectory* dir) {

  //Check directory ptr
  if(!dir) {
    cout << "RemoveDir : ERROR: Directory ptr is NULL" << endl;
    return;
  }

  //Remove it (from ptr to dir, get mother then delete by name)
  cout << "RootManager::RemoveDir : Removing " << dir->GetPath() << endl;
  static_cast<TDirectory*>( dir->GetMother() )->rmdir(dir->GetName());

}


//Add generic object to manager in directory specified
TObject* RootManager::Add(TDirectory* dir, TObject *obj) {

  //Check pointers
  if(!dir) {
    cout << "Add : ERROR: Directory pointer is NULL" << endl;
    return 0;
  }
  if(!obj) {
    cout << "Add : ERROR: Object pointer is NULL" << endl;
    return 0;
  }

  //Handle TTree special case
  if (IsATree(obj)) ((TTree*)obj)->SetDirectory(dir);

  //Handle histogram special case
  else if (IsAHist(obj)) ((TH1*)obj)->SetDirectory(dir);

  //Otherwise handle generic case (e.g. no SetDirectory function available)
  else dir->Add(obj, true);

  return obj;
}


//Get object from manager
TObject* RootManager::Get(TDirectory* dir, string objName)
{

  TObject* obj = 0;

  //Check dir pointer
  if(dir) {

    //Get object from directory
    obj = dir->FindObject(objName.c_str());

    //Check found object
    if(!obj) {
      cout << "Get : ERROR : No object called " << objName << " in dir " << dir->GetPath() << endl;
    }

  }
  else {
    cout << "Get : ERROR: Directory pointer is NULL" << endl;
  }

  return obj;

}


//Check if object exists
bool RootManager::Exists(TDirectory* dir, string objName)
{

  //  try {
    auto obj = Get(dir,objName);
    if(obj) return true;
    // }
  //  catch ( cet::exception e) {} //Ignore error thrown if not found
 
  return false;

}


// Remove an object from specified directory
void RootManager::Remove(TDirectory* dir, string objName) {

  //Check dir pointer
  if(dir) {

    //Get object from directory
    TObject* obj = dir->FindObject(objName.c_str());

    //Check found object and delete
    if(obj) {
      cout << "RootManager::Remove : Removing " << obj->GetName() << endl;
      obj->Delete();
    }
    else {
      cout << "Remove : ERROR : No object called " << objName << " in dir " 
        << dir->GetPath() << endl;
    }

  }
  else {
    cout << "Remove : ERROR: Directory pointer is NULL" << endl;
  }

}


//Clear all empty objects
void RootManager::ClearEmpty(TDirectory* dir,bool deleteDirectories) {

  //Check directory ptr is valid
  if( ! dir ) {    
    cout << "ClearEmpty : ERROR: Directory pointer is NULL" << endl;
    return;
  }


  //
  // Clear empty objects
  //

  //Loop through all objects
  vector<TObject*> objects = GetObjectList(dir); //GetObjectList is recursive
  for(vector<TObject*>::iterator it_obj=objects.begin() ; it_obj!=objects.end() ; ++it_obj) {
    TObject* obj = *it_obj;
    if(obj) {

      //Check if object is empty and remove if so (handle different ROOT classes separately)
      if (IsATree(obj)) {
        if( ((TTree*)obj)->GetEntries() < 1. ) {
          //cout << "RootManager::ClearEmpty : Removing " << obj->GetName() << endl;
          obj->Delete();
        }
      }
      else if (IsAHist(obj)) {
        if( ((TH1*)obj)->GetEntries() < 1. ) {
          //cout << "RootManager::ClearEmpty : Removing " << obj->GetName() << endl;
          obj->Delete();
        }
      }
      else if ( IsAGraph(obj)) {
        if( ((TGraph*)obj)->GetN() < 1. ) {
          //cout << "RootManager::ClearEmpty : Removing " << obj->GetName() << endl;
          ((TTree*)obj)->Delete();
        }
      }

    }

  }//end it_obj


  //
  // Clear empty directories
  //

  //Loop through all subdirectories and delete if empty
  //Do this after deleting objetcs to get the real picture of what directories are empty
  if(deleteDirectories) {
    ClearEmptyDirs(dir);
  }


}


//Recursive function to search through TDirectory structure and populate list of subdirectories
void RootManager::GetObjects(TDirectory* dir, vector<TObject*>& objects, string exp) {

  if(dir) {

    //Loop through objects in directory
    TIter next(dir->GetList());
    TObject *obj;
    while ((obj = (TObject*)next())) {

      //If object is a directory, loop again
      if (obj->InheritsFrom("TDirectory")) {
        GetObjects((TDirectory*)obj, objects, exp);
      }
      //Otherwise is an object
      else {
        //Add object to vector if name matches expression provided (default is "*" e.g. all)
        if( StringCmpWild(string(obj->GetName()),exp) ) {
          objects.push_back(obj);
          //cout << dir->GetName() << " : " << obj->GetName() << endl;
        }
      }

    }//end while

  }//end if dir
  else {
    cout << "GetObjects : Directory pointer is NULL" << endl;
  } 

}


//Recursive function to search through TDirectory structure and populate list of subdirectories
void RootManager::GetDirs(TDirectory* dir, vector<TDirectory*>& dirs, bool usePath, string exp) {

  if(dir) {

    //Loop through objects in directory
    TIter next(dir->GetList());
    TObject *obj;
    while ((obj = (TObject*)next())) {

      //Check if object is a directory
      if (obj->InheritsFrom("TDirectory")) {

        auto foundDir = static_cast<TDirectory*>(obj);

        //Add object to vector if name or path matches expression provided (default is "*" e.g. all)
        string stringToCheck = usePath ? foundDir->GetPath() : foundDir->GetName(); //Toggle whether checking name or path
        if( StringCmpWild(stringToCheck,exp) ) {
          dirs.push_back(foundDir);
        }

        //Check its subdirectories
        GetDirs(foundDir, dirs, usePath, exp);
      }

    }
  }
  else {
    cout << "GetDirs : Directory pointer is NULL" << endl;
  } 

}

vector<TDirectory*> RootManager::GetDirListByName(TDirectory* dir, string nameExp) {

  //Call recursive directories search - this populates the directories vector
  vector<TDirectory*> dirs;
  GetDirs(dir, dirs, false, nameExp); //FALSE -> string compare name not path

  //Return resulting populated vector
  return dirs;
}


vector<TDirectory*> RootManager::GetDirListByPath(TDirectory* dir, string pathExp) {

  //Call recursive directories search - this populates the directories vector
  vector<TDirectory*> dirs;
  GetDirs(dir, dirs, true, pathExp); //TRUE -> string compare path not name

  //Return resulting populated vector
  return dirs;
}


vector<string> RootManager::GetDirPathsListByName(TDirectory* dir, string nameExp) {

  //Get vector of directories
  vector<TDirectory*> dirs = GetDirListByName(dir, nameExp);

  //Get vector of object names from vector of objects
  return NameVectFromDirVect(dirs);

}


vector<TObject*> RootManager::GetObjectListByName(TDirectory* dir, string nameExp) {

  //Call recursive directories search - this populates the objects vector
  vector<TObject*> objects;
  GetObjects(dir, objects, nameExp);

  //Return resulting populated vector
  return objects;

}


vector<string> RootManager::GetObjectNamesList(TDirectory* dir) {

  //Get vector of objects
  vector<TObject*> objects = GetObjectList(dir);

  //Get vector of object names from vector of objects
  return NameVectFromObjectVect(objects);

}


//Loop through vector of objects and fill vector with their names
vector<string> RootManager::NameVectFromObjectVect(const vector<TObject*> & objects) {
  vector<string> objectNames;
  for(auto obj : objects ) {
    if(obj) objectNames.push_back(string(obj->GetName()));
  }
  return objectNames;
}


//Loop through vector of directories and fill vector with their names
//Need this as cannot cast vector<Derived*> to vector<Base*>
vector<string> RootManager::NameVectFromDirVect(const vector<TDirectory*> & dirs) {
  vector<string> dirNames;
  for(auto dir : dirs ) {
    if(dir) dirNames.push_back(string(dir->GetName()));
  }
  return dirNames;
}


//Recursive function to delete empty directories
bool RootManager::ClearEmptyDirs(TDirectory* dir) {

  //Check input dir
  if(dir) {

    //Loop through objects in directory
    TIter next(dir->GetList());
    TObject *obj;
    bool dirIsEmpty = true;
    while ((obj = (TObject*)next())) {

      //Check if object is a directory
      if (obj->InheritsFrom("TDirectory")) {

        //Clear contents of subdir if empty
        auto subDir = static_cast<TDirectory*>(obj);
        bool subDirEmpty = ! ClearEmptyDirs( subDir ); //Recursive call

        //If subdir is empty, delete it
        if( subDirEmpty ) {
          //cout << "Deleting empty subdir \"" << subDir->GetName() << "\" of \"" << dir->GetPath() << "\"" << endl;
          dir->rmdir(subDir->GetName());
        }
        //Otherwise, if subdir is not empty, this dir is not empty
        else {
          dirIsEmpty = false;
        }

      }
      else {
        //Found something, this dir is not empty
        dirIsEmpty = false;
      }

    }//while

    //Delete contents of this directory if it didn't contain anything except empty subdirs
    if( dirIsEmpty ) {
      //cout << "Deleting contents of directory \"" << dir->GetPath() << "\"" << endl;
      dir->Close();
      return false;
    }
    
  }
  else {
    cout << "GetDirs : Directory pointer is NULL" << endl;
  } 

  return true;

}


// Call Sumw2 on all hists
void RootManager::Sumw2() {
  cout << "RootManager::Sumw2" << endl;

  //Loop through all histograms
  auto hists = GetObjectList<TH1*>();
  for(vector<TH1*>::iterator it_hist=hists.begin() ; it_hist!=hists.end() ; ++it_hist) {

    TH1* hist = *it_hist;
    if(hist) {

      //Initialize the array of weights if it has not been done already
      if ( !hist->GetSumw2N() ) hist->Sumw2();
      cout << "Sumw2: " << hist->GetName() << endl;

    }
  }

}


//Check if object is a tree
bool RootManager::IsATree(TObject* obj) {
  bool result = false;
  if(obj) if ( obj->InheritsFrom("TTree") ) result = true;
  return result;
}


//Check if object is a histogram
bool RootManager::IsAHist(TObject* obj) {
  bool result = false;
  if(obj) if ( obj->InheritsFrom("TH1") ) result = true;
  return result;
}


//Check if object is a profile (note: TProfile will also pass IsAHist, as also derives from TH1)
bool RootManager::IsAProfile(TObject* obj) {
  bool result = false;
  if(obj) if ( obj->InheritsFrom("TProfile") ) result = true;
  return result;
}


//Check if object is a graph
bool RootManager::IsAGraph(TObject* obj) {
  bool result = false;
  if(obj) {
    if ( obj->InheritsFrom("TGraph") ) result = true;
  }
  else {
    cout << "IsAGraph : Object pointer is NULL" << endl;
  }
  return result;
}


//Draw a histogram or graph on canvas with default options
TCanvas* RootManager::DrawOnCanvas(TDirectory* dir, string objName, string cName) {

  //Get object and check it
  auto obj = Get(dir,objName);
  if( ! ( IsAHist(obj) || IsAGraph(obj) ) ){
    cout << "DrawOnCanvas : Object must be a histogram or graph" << endl;
    return 0;
  }

  //Create canvas to display the object and wire lines on
  auto title = obj->GetTitle();
  auto canvas = static_cast<TCanvas*>( Add(dir, new TCanvas(cName.c_str(),title,600,600) ) );

  //Change directory to canvas so that histo and TLines will be drawn there
  canvas->cd();

  //Draw object on canvas
  if(IsAHist(obj)) obj->Draw("CONT0");
  else if(IsAGraph(obj)) obj->Draw("AP");

  //Change back to top directory
  rootFile->cd();

  //Return pointer to canvas
  return canvas;

}


//String compare with wildcard
//Stolen from: http://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
bool RootManager::StringCmpWild(string str, string exp) {

  const char * wild = exp.c_str();
  const char * charString = str.c_str();
  const char *cp = NULL, *mp = NULL;

  while ((*charString) && (*wild != '*')) {
    if ((*wild != *charString) && (*wild != '?')) {
      return false;
    }
    wild++;
    charString++;
  }

  while (*charString) {
    if (*wild == '*') {
      if (!*++wild) {
        return true;
      }
      mp = wild;
      cp = charString+1;
    } else if ((*wild == *charString) || (*wild == '?')) {
      wild++;
      charString++;
    } else {
      wild = mp;
      charString = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}
