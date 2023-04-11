
#ifndef ROKZ_FILE_H
#define ROKZ_FILE_H

#include "common.hpp"
#include "utility.hpp"
#include "binary_IO.hpp"


namespace rokz { 

  // ----------------------------------------------------------------------------------------------
  //                                            
  // ----------------------------------------------------------------------------------------------
  inline size_t SizeOf_file (const std::string& fn) {
    //printf  ( "%s[%s]\n", __FUNCTION__, fn.c_str());    
    if (std::FILE* f = std::fopen (fn.c_str(), "r")) {
      //printf  ( "[f is good]\n" );    
      std::shared_ptr<std::FILE> sp (f, std::fclose); 
      if (0 == std::fseek (sp.get(), 0, SEEK_END)) 
	return  std::ftell (sp.get()); 
    }
    return 0; 
  }

  // ----------------------------------------------------------------------------------------------
  //                                            
  // ----------------------------------------------------------------------------------------------
  template<template<typename> class Seq, typename Ty>
  inline Seq<Ty>& From_file (Seq<Ty>& out, const std::string& fname, bool resize = true) {

    if (auto sizeOf_file = SizeOf_file (fname)) {

      if (resize) { 
        size_t num_el = sizeOf_file / sizeof(Ty);
        out.resize (num_el); 
      }
      else {
        size_t existing_size = sizeof(Ty) * out.size (); 
        if (sizeOf_file > existing_size) { 
          return out; // do nothing 
        }
      }

      ReadStream::Ref rs = CreateReadFileStream (fname);

      if (rs) {
	rs->Read (&out[0], sizeOf_file); 
      }
    }
    return out; 
  } 


}
#endif
