// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
/* ######################################################################

   Debian Index Files
   
   There are three sorts currently
   
   Package files that have File: tags
   Package files that don't (/var/lib/dpkg/status)
   Source files
   
   ##################################################################### */
									/*}}}*/
#ifndef PKGLIB_DEBINDEXFILE_H
#define PKGLIB_DEBINDEXFILE_H

#include <apt-pkg/indexfile.h>
#include <apt-pkg/cacheiterators.h>
#include <apt-pkg/pkgcache.h>
#include <apt-pkg/srcrecords.h>

#include <string>

class OpProgress;
class pkgAcquire;
class pkgCacheGenerator;

class debStatusIndex : public pkgDebianIndexRealFile
{
   void * const d;
protected:
   virtual std::string GetArchitecture() const APT_OVERRIDE;
   virtual std::string GetComponent() const APT_OVERRIDE;
   virtual uint8_t GetIndexFlags() const APT_OVERRIDE;

public:

   virtual const Type *GetType() const APT_OVERRIDE APT_CONST;

   // Interface for the Cache Generator
   virtual bool HasPackages() const APT_OVERRIDE {return true;};
   // Abort if the file does not exist.
   virtual bool Exists() const APT_OVERRIDE {return true;};

   virtual pkgCacheListParser * CreateListParser(FileFd &Pkg) APT_OVERRIDE;

   debStatusIndex(std::string const &File);
   virtual ~debStatusIndex();
};

class debPackagesIndex : public pkgDebianIndexTargetFile
{
   void * const d;
protected:
   virtual uint8_t GetIndexFlags() const APT_OVERRIDE;

public:
   virtual const Type *GetType() const APT_OVERRIDE APT_CONST;

   // Stuff for accessing files on remote items
   virtual std::string ArchiveInfo(pkgCache::VerIterator const &Ver) const APT_OVERRIDE;

   // Interface for the Cache Generator
   virtual bool HasPackages() const APT_OVERRIDE {return true;};

   debPackagesIndex(IndexTarget const &Target, bool const Trusted);
   virtual ~debPackagesIndex();
};

class debTranslationsIndex : public pkgDebianIndexTargetFile
{
   void * const d;
protected:
   virtual std::string GetArchitecture() const APT_OVERRIDE;
   virtual uint8_t GetIndexFlags() const APT_OVERRIDE;
   virtual bool OpenListFile(FileFd &Pkg, std::string const &FileName) APT_OVERRIDE;
   APT_HIDDEN virtual pkgCacheListParser * CreateListParser(FileFd &Pkg) APT_OVERRIDE;

public:

   virtual const Type *GetType() const APT_OVERRIDE APT_CONST;

   // Interface for the Cache Generator
   virtual bool HasPackages() const APT_OVERRIDE;

   debTranslationsIndex(IndexTarget const &Target);
   virtual ~debTranslationsIndex();
};

class debSourcesIndex : public pkgDebianIndexTargetFile
{
   void * const d;
   virtual uint8_t GetIndexFlags() const APT_OVERRIDE;
   virtual bool OpenListFile(FileFd &Pkg, std::string const &FileName) APT_OVERRIDE;
   APT_HIDDEN virtual pkgCacheListParser * CreateListParser(FileFd &Pkg) APT_OVERRIDE;

   public:

   virtual const Type *GetType() const APT_OVERRIDE APT_CONST;

   // Stuff for accessing files on remote items
   virtual std::string SourceInfo(pkgSrcRecords::Parser const &Record,
			     pkgSrcRecords::File const &File) const APT_OVERRIDE;

   // Interface for the record parsers
   virtual pkgSrcRecords::Parser *CreateSrcParser() const APT_OVERRIDE;

   // Interface for the Cache Generator
   virtual bool HasPackages() const APT_OVERRIDE {return false;};

   debSourcesIndex(IndexTarget const &Target, bool const Trusted);
   virtual ~debSourcesIndex();
};

class debDebPkgFileIndex : public pkgDebianIndexRealFile
{
   void * const d;
   std::string DebFile;

protected:
   virtual std::string GetComponent() const APT_OVERRIDE;
   virtual std::string GetArchitecture() const APT_OVERRIDE;
   virtual uint8_t GetIndexFlags() const APT_OVERRIDE;
   virtual bool OpenListFile(FileFd &Pkg, std::string const &FileName) APT_OVERRIDE;
   APT_HIDDEN virtual pkgCacheListParser * CreateListParser(FileFd &Pkg) APT_OVERRIDE;

public:
   virtual const Type *GetType() const APT_OVERRIDE APT_CONST;

   /** get the control (file) content of the deb file
    *
    * @param[out] content of the control file
    * @param debfile is the filename of the .deb-file
    * @return \b true if successful, otherwise \b false.
    */
   static bool GetContent(std::ostream &content, std::string const &debfile);

   // Interface for the Cache Generator
   virtual bool HasPackages() const APT_OVERRIDE {return true;}
   virtual pkgCache::PkgFileIterator FindInCache(pkgCache &Cache) const APT_OVERRIDE;

   // Interface for acquire

   debDebPkgFileIndex(std::string const &DebFile);
   virtual ~debDebPkgFileIndex();
};

class debDscFileIndex : public pkgDebianIndexRealFile
{
   void * const d;
 public:
   virtual const Type *GetType() const APT_OVERRIDE APT_CONST;
   virtual pkgSrcRecords::Parser *CreateSrcParser() const APT_OVERRIDE;
   virtual bool HasPackages() const APT_OVERRIDE {return false;};

   debDscFileIndex(std::string const &DscFile);
   virtual ~debDscFileIndex();
};

class debDebianSourceDirIndex : public debDscFileIndex
{
 public:
   virtual const Type *GetType() const APT_OVERRIDE APT_CONST;
};

#endif
