
#include "binary_IO.hpp"
#include "utility.hpp"
//#include <cstdio>

namespace rokz {

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  WriteStream::Ref CreateWriteMemStream (void* mem, size_t len) {
    //
    struct write_mem_stream_impl : public write_stream {

      write_mem_stream_impl (void* src, size_t len)
        : write_stream()
        , max_size     (len)
        , offs        (0)
        , mem(reinterpret_cast<unsigned char*>(src)) {
      }
    
      virtual ~write_mem_stream_impl () { HERE("BAI");
      }
      //
      void SetPos (ptrdiff_t offset, SeekMode mode) {

        switch (mode) {
          // except if >  max_size
        case SeekMode::Abs:
          offs = offset; 
          break;
          //
        case SeekMode::Rel:
          if (ut::in_bounds_incl<size_t> (offs+offset, 0, max_size))
            offs += offset; 
          break;
          // 
        case SeekMode::End:
          if (ut::in_bounds_excl<size_t> (offs + offset , 0, max_size))
            offs = max_size + offset;
          break;
        }      
      }
    
      // 
      ptrdiff_t GetPos () {
        return offs; 
      }
      //
      size_t Write (const void* src, size_t len) {

        const unsigned char* ucsrc = reinterpret_cast<const unsigned char*> (src);

        size_t write_len = len; 

        if (!ut::in_bounds_incl<size_t> (offs+len, 0, max_size)) {
          printf ("[%s:|%i] max_size:%zu  \n",   __FUNCTION__, __LINE__, max_size);
          printf ("[%s:|%i] offs:%zu  \n",   __FUNCTION__, __LINE__, offs);
          printf ("[%s:|%i] len:%zu  \n",   __FUNCTION__, __LINE__, len);
          assert (false); 

          write_len = max_size - offs;


        }
      
        std::copy (ucsrc, ucsrc+write_len, mem+offs); 
        offs += write_len; 

        return write_len; 

      }
      // 
      size_t         max_size; 
      size_t         offs;
      unsigned char* mem; 
    
    };
  
    return std::make_shared<write_mem_stream_impl>(mem, len); 
  }

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  ReadStream::Ref CreateReadMemStream  (const void* mem, size_t len) {
    //
    struct read_mem_stream_impl : public read_stream {
      //
      read_mem_stream_impl (const void* src, size_t len)
        : read_stream ()
        , max_size (len)
        , offs(0)
        , mem (reinterpret_cast<const  unsigned char*>(src)) {

      }

      virtual ~read_mem_stream_impl () { HERE("bai");
      }

      void SetPos (ptrdiff_t offset, SeekMode mode) {
        switch (mode) {
          // except if >  max_size
        case SeekMode::Abs:
          offs = offset; 
          break;
	
        case SeekMode::Rel:
	
          if (ut::in_bounds_incl<size_t> (offs+offset, 0, max_size))
            offs += offset; 

          break;

        case SeekMode::End:

          if (ut::in_bounds_excl<size_t> (offs + offset , 0, max_size))
            offs = max_size + offset;

          break;
        }}

      
      ptrdiff_t GetPos () {
        return offs;
      }

      //
      size_t Read (void* out, size_t len) {
        unsigned char*  ucout = reinterpret_cast<unsigned char*>(out);

        size_t read_len = len; 
        if ((offs + len) > max_size)
          read_len = max_size - offs;

        std::copy (mem+offs, mem+offs+read_len, ucout); 

        offs += read_len;
        return read_len; 
      }

    protected:
      size_t               max_size; 
      size_t               offs; 
      const unsigned char* mem;
    
    };

    return std::make_shared<read_mem_stream_impl>(mem, len); 

  }

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  WriteStream::Ref CreateWriteFileStream  (const std::string& fname) {

    struct write_file_str_imp: public write_stream {

      std::shared_ptr<std::FILE> file;
    
      write_file_str_imp (const std::string& fname): write_stream (), file (std::fopen (fname.c_str(),  "wb"), std::fclose) {
      }
    
      virtual ~write_file_str_imp () {

      }
      //
      //
      void SetPos (ptrdiff_t offset, SeekMode mode) {

        switch (mode) {
          // except if >  max_size
        case SeekMode::Abs:

          std::fseek (file.get(), offset, SEEK_SET);

          break;
          //
        case SeekMode::Rel:
          std::fseek(file.get(), offset, SEEK_CUR); 
          break;
          // 
        case SeekMode::End:
          std::fseek(file.get(), offset, SEEK_END); 
          break;
        }      
      }
    
      // 
      ptrdiff_t GetPos () {
        return std::ftell (file.get()); 
      }

      //
      size_t Write (const void* src, size_t len) {
        return std::fwrite (src, 1, len, file.get());
      }
    
    };

    return std::make_shared<write_file_str_imp> (fname);
  }

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  ReadStream::Ref CreateReadFileStream (const std::string& fname) {
    //
    struct read_file_str_impl : public read_stream {
      //
    public: 

      read_file_str_impl ( std::shared_ptr<std::FILE> f_, size_t szmax)
        : f        (f_)
        , max_size (szmax)
        , offs     (0) {
      }
    
      virtual ~read_file_str_impl () {
        f.reset (); 
      }
      // 
      virtual void SetPos (ptrdiff_t offset, SeekMode mode) {

        switch (mode) {
          // except if >  max_size
        case SeekMode::Abs:
          std::fseek ( f.get(), offset, SEEK_SET); 
          break;
          //
        case SeekMode::Rel:
          if (ut::in_bounds_incl<size_t> (offs+offset, 0, max_size))
            std::fseek (f.get(), offset, SEEK_CUR); 
          break;
          // 
        case SeekMode::End:
          if (ut::in_bounds_excl<size_t> (offs + offset , 0, max_size))
            std::fseek (f.get(), offset, SEEK_END); 
	  
          break;
        }      
      }

      // 
      virtual ptrdiff_t GetPos () {
        return std::ftell (f.get()); 
      }

      //
      virtual size_t Read (void* out, size_t len) {
        return  std::fread (out, sizeof(unsigned char), len, f.get()); 
      }

    protected: // 
      //
      std::shared_ptr<std::FILE> f;
      size_t                max_size; 
      size_t                offs; 
    
    };


    //printf ("%s ", __FUNCTION__);
    ReadStream::Ref ref (nullptr); 
    if (std::FILE* f = std::fopen (fname.c_str (), "r")) {
     
      std::fseek (f, 0, SEEK_END);
      size_t sz = std::ftell (f);
      std::fseek (f, 0, SEEK_SET); 
      ref = std::make_shared<read_file_str_impl> (std::shared_ptr<std::FILE> (f, std::fclose), sz); 
      //ref.reset (new read_file_str_impl (std::shared_ptr<std::FILE> (f, std::fclose), sz));
      //printf ("[OK]\n", fname.c_str());
      return ref; 
    }

    printf ("%s [ERROR] --> \"%s\" not found\n",  __FUNCTION__,  fname.c_str());
    return ref;  
  }



  
} // af 

