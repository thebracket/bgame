#include "png_writer.h"
#include <iostream>
#include <cstdlib>

using namespace std;

png_writer::png_writer ( string nFilename, int w, int h )
{
     filename = nFilename;
     height = h;
     width = w;
     std::cout << "Making PNG size " << w << " x " << h << "\n";

     fp = fopen ( filename.c_str(), "wb" );
     if ( !fp ) {
          cout << "WARNING: Writing to PNG (" << filename << ") failed.\n";
     } else {
          // Initialize Write Structure
          png_ptr = png_create_write_struct ( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
          if ( png_ptr == NULL ) {
               cout << "WARNING: Creating PNG structure failed (" << filename << ").\n";
          }

          // Iniitalize info structure
          info_ptr = png_create_info_struct ( png_ptr );
          if ( info_ptr == NULL ) {
               cout << "WARNING: Creating PNG info pointer failed (" << filename << ").\n";
          }

          // Setup C exception handling
          if ( setjmp ( png_jmpbuf ( png_ptr ) ) ) {
               cout << "WARNING: Setting up libpng exception handler failed.\n";
          }

          png_init_io ( png_ptr, fp );

          // Write header (32 bit colour depth)
          png_set_IHDR ( png_ptr, info_ptr, width, height,
                         8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );

          png_write_info ( png_ptr, info_ptr );

          // Setup internal buffer to hold image data
          buf = ( png_bytep* ) malloc ( sizeof ( png_bytep ) * height );
          for ( int i=0; i<height; i++ )
               buf[i] = ( png_byte* ) malloc ( png_get_rowbytes ( png_ptr,info_ptr ) );
     }
}

void png_writer::setPixel ( int x, int y, png_byte r, png_byte g, png_byte b, png_byte a )
{
     //std::cout << x << "," << y << "\n";
     int offset = x*4;
     png_bytep row = buf[y];

     row[offset] = r;
     row[offset+1] = g;
     row[offset+2] = b;
     row[offset+3] = a;
}

void png_writer::save()
{
     // Iterate the buffer in rows and save the image
     png_write_image ( png_ptr, buf );
     //for ( int y=0; y<height; y++ )
     //     free ( buf[y] );
     //free ( buf );
}

png_writer::~png_writer()
{
     png_write_end ( png_ptr, NULL );
     if ( fp != NULL ) fclose ( fp );
     if ( info_ptr != NULL ) png_free_data ( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
     if ( png_ptr != NULL ) png_destroy_write_struct ( &png_ptr, ( png_infopp ) NULL );
}
