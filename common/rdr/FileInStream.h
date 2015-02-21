/* Copyright (C) 2013 D. R. Commander.  All Rights Reserved.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */

#ifndef __RDR_FILEINSTREAM_H__
#define __RDR_FILEINSTREAM_H__

#include <stdio.h>
#include <sys/time.h>
#include <errno.h>

#include <rdr/Exception.h>
#include <rdr/InStream.h>

#define BUFSIZE 131072

namespace rdr {

  class FileInStream : public InStream {

  public:

    FileInStream(char *fileName) {
      file = fopen(fileName, "rb");
      if (!file) throw Exception(strerror(errno));
      ptr = end = b;
    }

    ~FileInStream(void) {
      if (file) {
        fclose(file);
        file = NULL;
      }
    }

    double getReadTime() { return tRead; }
    void resetReadTime() { tRead = 0.0; }

    void reset(void) {
      if (!file) throw Exception("File is not open");
      if (fseek(file, 0, SEEK_SET) != 0)
        throw Exception(strerror(errno));
      ptr = end = b;
    }

    int pos() { return ptr - b; }

  protected:
    int overrun(int itemSize, int nItems, bool wait = true);

  private:
    U8 b[BUFSIZE];
    FILE *file;
    double tRead;
  };

} // end of namespace rdr

#endif
