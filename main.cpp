#include <id3/tag.h>
#include <iostream>
#include <filesystem>


void remove_comment( const std::filesystem::path & file )
{
    ID3_Tag tag( file.c_str() );

    bool frame_found = false;
    for( ID3_Frame * frame;  nullptr != (frame = tag.Find(ID3FID_COMMENT)); )
    {
        frame_found = true;
        tag.RemoveFrame( frame );
        std::cout << "File '"
                  << file.c_str()
                  << "' : ID3FID_COMMENT found, removed.\n"
        ;
    }
    if( ! frame_found )
    {
        std::cout << "File '"
                  << file.c_str()
                  << "' : no ID3FID_COMMENT found.\n"
        ;
        return;
    }

    tag.Update();
}

void process_files( const char * path )
{
    for( auto & dir_entry : std::filesystem::recursive_directory_iterator(path) )
    {
        if( ! dir_entry.is_regular_file() )
            continue;

        std::filesystem::path file = dir_entry.path();
        remove_comment( file );
    }
}


int main( int argc, char ** argv )
{
    if( argc < 2 )
        return 1;

    for( int i = 1; i < argc; ++i )
        process_files( argv[i] );

    return 0;
}
