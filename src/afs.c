/*****************************************************************************
**
**  AFS library
**
**  Creation date:  2018/02/14
**  Created by:     Ole Liabo
**
**
**  Copyright (c) 2018 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//---------------------------------------------------------------------------- 
/*! \defgroup afs Archival File System
 *  \brief Functions for managing an archival file system
 *
 *  The afs library is a library for reading Archival File Systems (AFS). The 
 *  Archival File System has unique preservation capabilites, including support 
 *  for human readable data, file format ids and other types of metadata. A 
 *  design goal for AFS is to provide a completely self conatined file system, 
 *  containing all information needed to decede and understand its content in 
 *  the near and distant future. The AFS file system is primarily intended for 
 *  linear tape storeage mediums.
 *
 *  The key components in the AFS is the zero-reference-mark indicating the 
 *  start of the file system, the control-frame containing information about 
 *  how to decode the rest of the reel and the table-of-content pointing to all 
 *  files stored on the reel. The file system is organised into sectors, called 
 *  frames, where a frame occupies a rectangle on the storage medium surface. 
 *  The frame can contain either visual or digital data. The data on the frame 
 *  could be encoded using any kind of 2D barcode format. Current instances of
 *  AFS uses the boxinglib 2D barcode format.
 * 
 *  \section Definitions
 *
 *  sampled-image 
 *  \subsection AFS
 *  Archival File System.
 *  \subsection  human-readable-data 
 *  Data readable by the human eye. Typically a 2D image on paper or electronic display.
 *  \subsection  storage-medium 
 *  Physical object capable of keeping analog and/or digital information
 *  \subsection  file-format-id 
 *  Unique identifier for a file format.
 *  \subsection  file-format 
 *  Description of a bitstream organization for a particular purpose.
 *  \subsection  metadata 
 *  Information about the files stored in the AFS filesystem.
 *  \subsection  linear-tap-storage-medium 
 *  Storage medim where the bitream is stored and accessed in a linear fashion.
 *  \subsection  zero-reference-mark 
 *  Indicates the start of the data area on an AFS storage medium.
 *  \subsection  sector 
 *  Physical area on storage medium.
 */

typedef int remove_iso_compilers_warning; // Unused type definition to avoid warnings - ISO C requires a translation unit to contain at least one declaration
