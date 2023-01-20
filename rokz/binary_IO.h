
#ifndef ROKZ_BINARY_IO_H
#define ROKZ_BINARY_IO_H


#include "common.h"
#include "utility.h"
#include "rokz_types.h"

namespace rokz {

  //
  // 
  struct byte_stream {
    
    enum struct SeekMode : int  {
      Abs,  
      Rel,
      End, 

    }; 

    virtual void   SetPos (ptrdiff_t offs, SeekMode mode) = 0; 
    virtual size_t GetPos () = 0;

  protected:

    byte_stream () {}
  }; 


  //
  //
  struct reader {

    virtual size_t Read (void* out, size_t len) = 0;


    reader () {}
  };

  //
  // 
  struct writer {
  
    virtual size_t Write (const void* src, size_t len) = 0; 
    
  protected:
    writer () {} 
  };
  

  struct read_stream : public byte_stream, public reader, public ut::destructor {

  protected:
    read_stream () : byte_stream(), reader () {}
  }; 

  struct write_stream : public byte_stream, public writer, public ut::destructor {
  protected:
    write_stream () : byte_stream(), writer () {}
  };
  
  typedef std::shared_ptr<read_stream>  ReadStreamRef;
  typedef std::shared_ptr<write_stream> WriteStreamRef;
  
  
  WriteStreamRef CreateWriteMemStream (void* mem, size_t len);


  ReadStreamRef  CreateReadMemStream  (const void* mem, size_t len);
  ReadStreamRef  CreateReadFileStream (const std::string& fname); 



  
  
  
} // namespace af


#endif
