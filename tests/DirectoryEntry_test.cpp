#ifndef DE_TEST
#define DE_TEST
#include "test.hpp"
#include <iostream>
#include "../phat/DirectoryEntry.hpp"
#include "hwlib.hpp"
class DirectoryEntryLFNTest : public DirectoryEntry {
public:
	
	DirectoryEntryLFNTest( uint8_t data_[32], hwlib::string<0> & lfn, uint8_t parent ) : DirectoryEntry( data_, lfn, parent ) {}
	void testLFNDirectoryEntry( TestSuite & TestSuite ) {
		TestSuite.assertEquals( LFN, true, "LFN" );
		TestSuite.assertEquals( FAT_ATTR_TYPE, ( uint8_t ) 16, "testLFNDirectoryEntry - FAT_ATTR_TYPE" );
		TestSuite.assertEquals( CreatedTime10ms, ( uint8_t ) 151, "testLFNDirectoryEntry - CreatedTime10ms" );
		TestSuite.assertEquals( CreatedTime, ( uint16_t ) 21879, "testLFNDirectoryEntry - CreatedTime" );
		TestSuite.assertEquals( CreatedDate, ( uint16_t ) 19673, "testLFNDirectoryEntry - CreateDate" );
		TestSuite.assertEquals( AccessDate, ( uint16_t ) 19673, "testLFNDirectoryEntry - AccessDate" );
		TestSuite.assertEquals( UpdateTime, ( uint16_t ) 21880, "testLFNDirectoryEntry - UpdateTime" );
		TestSuite.assertEquals( UpdateDate, ( uint16_t ) 19673, "testLFNDirectoryEntry - UpdateDate" );
		TestSuite.assertEquals( FirstLogicalCluster, ( uint32_t ) 6, "testLFNDirectoryEntry - FirstLogicalCluster" );
		TestSuite.assertEquals( FileSize, ( uint32_t ) 0, "testLFNDirectoryEntry - FileSize" );
		TestSuite.assertEquals( getFirstLogicalCluster(), ( uint32_t ) 6, "testLFNDirectoryEntry - getFirstLogicalCluster()" );
		TestSuite.assertEquals( getFileSize(), ( uint32_t ) 0, "testLFNDirectoryEntry - getFileSize()" );
		TestSuite.assertEquals( isFile(), false, "testLFNDirectoryEntry - isFile()" );
		TestSuite.assertEquals( isADirectory(), true, "testLFNDirectoryEntry - isADirectory()" );
		TestSuite.assertEquals( getParentIndex(), ( uint8_t ) 0, "testLFNDirectoryEntry - getParentIndex()" );
	}
};

class DirectoryEntryTest : public DirectoryEntry {
public:
	DirectoryEntryTest( uint8_t data_[32], uint8_t parent ) : DirectoryEntry( data_, parent ) {}
	void testDirectoryEntry( TestSuite & TestSuite ) {
		TestSuite.assertEquals( LFN, false, "LFN" );
		TestSuite.assertEquals( FAT_ATTR_TYPE, ( uint8_t ) 32, "testDirectoryEntry - FAT_ATTR_TYPE" );
		TestSuite.assertEquals( CreatedTime10ms, ( uint8_t ) 70, "testDirectoryEntry - CreatedTime10ms" );
		TestSuite.assertEquals( CreatedTime, ( uint16_t ) 21894, "testDirectoryEntry - CreatedTime" );
		TestSuite.assertEquals( CreatedDate, ( uint16_t ) 19673, "testDirectoryEntry - CreatedDate" );
		TestSuite.assertEquals( AccessDate, ( uint16_t ) 19673, "testDirectoryEntry - AccessDate" );
		TestSuite.assertEquals( UpdateTime, ( uint16_t ) 21899, "testDirectoryEntry - UpdateTime" );
		TestSuite.assertEquals( UpdateDate, ( uint16_t ) 19673, "testDirectoryEntry - UpdateDate" );
		TestSuite.assertEquals( FirstLogicalCluster, ( uint32_t ) 8, "testDirectoryEntry - FirstLogicalCluster" );
		TestSuite.assertEquals( FileSize, ( uint32_t ) 36, "testDirectoryEntry - FileSize" );
		TestSuite.assertEquals( getFirstLogicalCluster(), ( uint32_t ) 8, "testDirectoryEntry - getFirstLogicalCluster()" );
		TestSuite.assertEquals( getFileSize(), ( uint32_t ) 36, "testDirectoryEntry - getFileSize()" );
		TestSuite.assertEquals( isFile(), true, "testDirectoryEntry - isFile()" );
		TestSuite.assertEquals( isADirectory(), false, "testDirectoryEntry - isADirectory()" );
		TestSuite.assertEquals( getParentIndex(), ( uint8_t ) 0, "testDirectoryEntry - getParentIndex()" );
	}
};
#endif