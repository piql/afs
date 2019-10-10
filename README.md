# Afslib

**afslib** is a library for reading Archival File Systems (**AFS**). The **AFS** has unique preservation capabilites, including support for human readable data, file format ids and other types of metadata. A design goal for **AFS** is to provide a completely self conatined file system, containing all information needed to decede and understand its content in the near and distant future. The **AFS** file system is primarily intended for **linear tape storeage** mediums.

The key components in the **AFS** is the **zero-reference-mark** indicating the start of the file system, the **control-frame** containing information about how to decode the rest of the reel and the **table-of-content** pointing to all files stored on the reel. The file system is organised into sectors, called **frames**, where a frame occupies a rectangle on the storage medium surface. The frame can contain either visual or digital data. The data on the frame could be encoded using any kind of 2D barcode format. Current instances of AFS uses the boxinglib 2D barcode format.

See LICENSE for software licensing.

This library contains a version of the third party library Mini-XML, which is Copyright 2003-2016 by Michael R Sweet.

<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#sec-1">1. Afslib</a></li>
<li><a href="#sec-2">2. Features</a></li>
<li><a href="#sec-3">3. Requirements</a></li>
<li><a href="#sec-4">4. Installation</a></li>
</ul>
</div>
</div>

# Features

-   Simple file system with unique preservation capabilities
-   Portable
-   TOC supports both digital and visual data
-   TOC supports file metadata in any format
-   TOC supports file format identification
-   Control frame supports self contained bootstrap for decoding rest of the reel

# Requirements

C language compiler supporting C99 (ISO/IEC 9899:1999).

A design goal is to have as few dependencies as possible.

# Installation

    git clone https://github.com/piql/afs
    cd afs
    ./autogen.sh
    ./configure LIBBOXING_DIR=<absolute path to boxinglib>
    make
