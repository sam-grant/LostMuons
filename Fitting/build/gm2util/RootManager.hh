//Manager class for ROOT objects.
//
//A TFile Must be provided an construction, and all objects registered with this manager will be stored in this file (even if other TFiles are subsequently opened). A objects are written to the file when "WriteToFile" is called.
//
//The file owner is responsible for keeping the file open as long as the manager is in use.
//
//A number of distinct ROOT managers can be opened as used with the same or different TFiles.
//
//Objects are stored in ROOT directories which may be nested. When adding or retrieving objects teh directory must be specified. When specifying a directory have three options (implemented with overloaded functions); 1) Directory ptr, 2) Directory path string, 3) Provide no directory and object will be stored at top-level of the file.
//
//For getters functions, additional functions voerloads with templates have been provied to provide casting to the derived ROOT type of the object (e.g. TH1F* rather than TObject*) to make it easier in user code to get the type you want. These will throw is casting is not possible, which is caught by the top-level of the art process.


#ifndef RootManager_hh
#define RootManager_hh

#include "TNamed.h"
//#include "TROOT.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TTree.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TList.h"

#include <iostream>
#include <string>
#include <map>
#include <memory>

using namespace std;

//NOTE: Not using smart ptrs here to keep flexible

class RootManager: public TNamed {

public:

  //Construction/destruction
  RootManager() = delete; //Remove default cosntructor, must provide ROOT file
  RootManager(string name, TFile* file);
  ~RootManager();

  //Manage file writing (not required if using TFileService, or you'll write twice!)
  void WriteToFile(); //Write to the ROOT file

  //
  // Directory handling
  //

  //Get and create dirs
  TDirectory* GetDir(TDirectory* parent, string dirRelPath, bool create=false);
  TDirectory* GetDir(string parentDirPath, string dirRelPath, bool create=false)
    { return GetDir(GetDir(rootFile,parentDirPath,create), dirRelPath, create); }
  TDirectory* GetDir(string dirAbsPath, bool create=false) 
    { return GetDir(rootFile, dirAbsPath, create); }

  //Remove dirs
  void RemoveDir(TDirectory* dir);
  void RemoveDir(string dirAbsPath) 
    { RemoveDir(GetDir(dirAbsPath)); } //Directory name or path

  //
  // Object handling
  //

  //Add objects to manager (and return ptr to it)
  TObject* Add(TDirectory* dir, TObject *obj);
  TObject* Add(string dirAbsPath, TObject *obj) 
    { return Add(GetDir(rootFile,dirAbsPath,true),obj); } //Create dir if doesn't exists
  TObject* Add(TObject *obj) 
    { return Add(rootFile,obj); } //Use ROOT file as directory

  //Remove objects from manager
  void Remove(TDirectory* dir, string objName);
  void Remove(string dirName, string objName) { Remove(GetDir(dirName),objName); }
  void Remove(string objName) { Remove(rootFile,objName); } //Use ROOT file as directory

  //Remove all objects that are empty from the specified directory and its subdirectories (e.g. this is recursive)
  //If "deleteDirectories" is true (it IS by default), also delete empty subdirs
  //WARNING: Take care using ClearEmpty() (e.g. no args version), as this will clear everything that is empty in the entire ROOT file
  //         If using TFileService to provide the ROOT file, this may includes stuff from other analyzers
  void ClearEmpty(TDirectory* dir, bool deleteDirectories=true);
  void ClearEmpty(string dirAbsPath, bool deleteDirectories=true) { ClearEmpty(GetDir(dirAbsPath,deleteDirectories)); }
  void ClearEmpty(bool deleteDirectories=true) { ClearEmpty(rootFile,deleteDirectories); } //Use ROOT file as directory

  //Object accessors
  //Can either call "Get" to return TObject* or Get<T> to dynamic cast to the specified type (e.g. T=TH1*,TTree* etc)
  //If can't cast (because the object is not that type or derived from it), exception is thrown
  TObject* Get(TDirectory* dir, string objName);
  template<class T> T Get(TDirectory* dir, string objName);
  TObject* Get(string dirName, string objName) 
    { return Get(GetDir(dirName), objName); }
  template<class T> T Get(string dirName, string objName) 
    { return Get<T>(GetDir(dirName), objName); }
  TObject* Get(string objName) 
    { return Get(rootFile,objName); } //Use ROOT file as directory
  template<class T> T Get(string objName) 
    { return Get<T>(rootFile,objName); }

  //Check an object exists
  bool Exists(TDirectory* dir, string objName);
  bool Exists(string dirName, string objName) 
    { return Exists(GetDir(dirName), objName); }
  bool Exists(string objName) 
    { return Exists(rootFile,objName); } //Use ROOT file as directory



  //
  // Directory list getters
  //

  //Get vector of directories
  vector<TDirectory*> GetDirList(TDirectory* dir)
    { return GetDirListByName(dir,"*"); } //Use by name get with all strings OK
  vector<TDirectory*> GetDirList(string dirName) 
    { return GetDirList(GetDir(dirName)); } //Use name instead of ptr
  vector<TDirectory*> GetDirList() 
    { return GetDirList(rootFile); } //Default to top-level

  //Get vectors of directories matching name string expression (wilcard * can be used)
  vector<TDirectory*> GetDirListByName(TDirectory* dir, string nameExp);
  vector<TDirectory*> GetDirListByName(string dirName, string nameExp) 
    { return GetDirListByName(GetDir(dirName), nameExp); } //Use name instead of ptr
  vector<TDirectory*> GetDirListByName(string nameExp) 
    { return GetDirListByName(rootFile, nameExp); } //Default to top-level

  //Get vectors of directories matching path string expression (wilcard * can be used)
  vector<TDirectory*> GetDirListByPath(TDirectory* dir, string pathExp);
  vector<TDirectory*> GetDirListByPath(string dirName, string pathExp) 
    { return GetDirListByPath(GetDir(dirName), pathExp); } //Use name instead of ptr
  vector<TDirectory*> GetDirListByPath(string pathExp) 
    { return GetDirListByPath(rootFile, pathExp); } //Default to top-level

  //Get vector of directory paths
  vector<string> GetDirPathsList(TDirectory* dir)
    { return GetDirPathsListByName(dir,"*"); } //Use get by name function with wildcard to get all
  vector<string> GetDirPathsList(string dirName) 
    { return GetDirPathsList(GetDir(dirName)); }
  vector<string> GetDirPathsList() 
    { return GetDirPathsList(rootFile); }

  //Get vectors of directory paths matching name string expression (wilcard * can be used)
  vector<string> GetDirPathsListByName(TDirectory* dir, string nameExp);
  vector<string> GetDirPathsListByName(string dirName, string nameExp) 
    { return GetDirPathsListByName(GetDir(dirName), nameExp); }
  vector<string> GetDirPathsListByName(string nameExp) 
    { return GetDirPathsListByName(rootFile, nameExp); }

  //
  // Object list getters
  //

  //Get vector of contained objects
  //Use template overloads to only get those objects matching the template type
  vector<TObject*> GetObjectList(TDirectory* dir) 
    { return GetObjectListByName(dir,"*"); } //"*" means return objects with any name
  template<class T> vector<T> GetObjectList(TDirectory* dir) 
    { return GetObjectListByName<T>(dir,"*"); }
  vector<TObject*> GetObjectList(string dirName) 
    { return GetObjectList(GetDir(dirName)); }
  template<class T> vector<T> GetObjectList(string dirName) 
    { return GetObjectList<T>(GetDir(dirName)); }
  vector<TObject*> GetObjectList() 
    { return GetObjectList(rootFile); }
  template<class T> vector<T> GetObjectList() 
    { return GetObjectList<T>(rootFile); }

  //Get vectors of contained objects matching name expression (can use * wilcard)
  //Include template overloads to only get those objects matching the template type
  vector<TObject*> GetObjectListByName(TDirectory* dir, string nameExp);
  template<class T> vector<T> GetObjectListByName(TDirectory* dir, string nameExp); 
  vector<TObject*> GetObjectListByName(string dirName, string nameExp) 
    { return GetObjectListByName(GetDir(dirName),nameExp); }
  template<class T> vector<T> GetObjectListByName(string dirName, string nameExp) 
    { return GetObjectListByName<T>(GetDir(dirName),nameExp); }
  vector<TObject*> GetObjectListByName(string nameExp) 
    { return GetObjectListByName(rootFile,nameExp); }
  template<class T> vector<T> GetObjectListByName(string nameExp) 
    { return GetObjectListByName<T>(rootFile,nameExp); } 

  //Get vectors of contained object names
  //Include template overloads to only get those objects matching the template type
  vector<string> GetObjectNamesList(TDirectory* dir); 
  template<class T> vector<string> GetObjectNamesList(TDirectory* dir); 
  vector<string> GetObjectNamesList(string dirName) 
    { return GetObjectNamesList(GetDir(dirName)); }
  template<class T> vector<string>  GetObjectNamesList(string dirName) 
    { return GetObjectNamesList<T>(GetDir(dirName)); }
  vector<string> GetObjectNamesList() 
    { return GetObjectNamesList(rootFile); }
  template<class T> vector<string>  GetObjectNamesList() 
    { return GetObjectNamesList<T>(rootFile); }


  //
  // Object vector <-> TList conversion
  //

  //Get TList from std::vector
  std::shared_ptr<TList> GetObjectList(const vector<TObject*> & objects) { return GetObjectList<TObject*>(objects); }
  template<class T> std::shared_ptr<TList> GetObjectList(const vector<T> & objects); 


  //
  // Merging
  //

  //Create a new histogram by merging the vector of histograms provided
  //If "ignoreEmpty" flag set, do not add any histograms with 0 entries to the merge (avoids issue with alphanumeric labels)
  template<class T> T MergeHists(const vector<T> & hists, std::string newName, bool ignoreEmpty=false);


  //
  // Misc
  //

  //Set sumw2 for weighted histograms
  void Sumw2();

  //Type checkers
  bool IsATree(TObject* obj);
  bool IsAHist(TObject* obj);
  bool IsAProfile(TObject* obj);
  bool IsAGraph(TObject* obj);

  //Draw on a object on a canvas in same directory
  TCanvas* DrawOnCanvas(TDirectory* dir, string objName, string cName);
  TCanvas* DrawOnCanvas(string dirName, string objName, string cName) 
    { return DrawOnCanvas(GetDir(dirName), objName, cName); }
  TObject* DrawOnCanvas(string objName, string cName) 
    { return DrawOnCanvas(rootFile,objName,cName); } //Use ROOT file as directory

  //Check file has been initialised
  bool fileWriteStarted() { return writeToFile; }


protected:

  bool StringCmpWild(string str, string exp);

  //Manage file writing
  void SetFile(TFile* file); //Define the ROOT file to be used

  //Underlying init for constructor
  void Init();

  //Recursive for all subdirectories in this directory
  void GetDirs(TDirectory* dir, vector<TDirectory*>& dirs, bool usePath=false, string exp="");

  //Recursive for all objects in this directory (including those in subdirectories)
  void GetObjects(TDirectory* dir, vector<TObject*>& objects, string exp="*");

  //Recursive function to clear empty directories (returns false if dir is empty, and thus deleted)
  bool ClearEmptyDirs(TDirectory* dir);

  //Functions to get vector of object/directory names from vector of objects/directories
  vector<string> NameVectFromObjectVect(const vector<TObject*> & objects);
  vector<string> NameVectFromDirVect(const vector<TDirectory*> & objects);

  //Member variables
  TFile* rootFile; //ROOT file to write to
  bool writeToFile;

};


//
// Implement template functions in header
//

//Get object from manager and try to dynamic cast it to template type
template<class T> T  RootManager::Get(TDirectory* dir, string objName)
{

  T returnPtr = 0;

  //Use regular get (without template) to get TOBject ptr
  TObject* obj = Get(dir,objName);

  //Check found object (don't throw as underlying function does it)
  if(obj) {

    //Cast object to template type
    returnPtr = dynamic_cast<T>(obj);

    //Check cast worked
    if(!returnPtr) {
      cout << "Get : ERROR : Could not cast " << objName << " in dir " << dir->GetPath() 
        << " to specified type" << endl;
    }

  }

  return returnPtr;

}


//Get vector of objects that can be cast to the template type
template<class T> vector<T> RootManager::GetObjectListByName(TDirectory* dir, string nameExp) {

  //Get vector of objects
  vector<TObject*> objects = GetObjectListByName(dir,nameExp);

  //Fill vector of objects that can be cast to specified type
  vector<T> castedObjects;
  for(vector<TObject*>::iterator it_obj=objects.begin() ; it_obj!=objects.end() ; ++it_obj) {
    TObject* obj = *it_obj;
    if(obj) { //Check obj ptr
      //Cast object and add to output vector if successful
      auto castedObj = dynamic_cast<T>( obj );
      if(castedObj) castedObjects.push_back(castedObj);
    }
  }

  //Return resulting populated vector
  return castedObjects;
}


template<class T> vector<string> RootManager::GetObjectNamesList(TDirectory* dir) {

  //Get vector of objects deriving from template type
  vector<TObject*> objects = GetObjectList<T>(dir);

  //Get vector of object names from vector of objects
  return NameVectFromObjectVect(objects);

}


template<class T> std::shared_ptr<TList> RootManager::GetObjectList(const vector<T> & objects) {

  std::shared_ptr<TList> list( new TList() );
  for( auto obj : objects ) list->Add(obj);
  return list;

}


template<class T> T RootManager::MergeHists(const vector<T> & hists, std::string newName, bool ignoreEmpty) {

  T newHist = NULL;

  //Check inout container
  if( hists.size() > 0 ) {

    //Create new histo from one of originals
    newHist = (T)hists.front()->Clone(newName.c_str()); 

    //Clear bin contents from original
    newHist->Reset(); 

    //Form TList of all hists to merge
    //If ignoring empty histos, don't add these to the list
    std::shared_ptr<TList> list;
    if(ignoreEmpty) {
      list.reset( new TList() );
      for( auto hist : hists ) {
        if( hist->GetEntries() ) list->Add(hist);
      }
    }
    else {
      list = this->GetObjectList(hists); //Can use generic function in this case (not histo-specific)
    }

    //Merge them into the new (reset) hist
    newHist->Merge( list.get() );

  }

  else {
    cout << "MergeHists : Empty vector of histograms provided" << endl;
  }

  return newHist;

}

#endif

