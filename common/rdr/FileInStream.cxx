/* Copyright (C) 2002-2005 RealVNC Ltd.  All Rights Reserved.
 * Copyright (C) 2013 D. R. Commander.  All Rights Reserved.
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

#include <rdr/Exception.h>
#include <rdr/FileInStream.h>

using namespace rdr;

extern double getTime();

int FileInStream::overrun(int itemSize, int nItems, bool wait) {

  if (itemSize > BUFSIZE)
    throw Exception("FileInStream overrun: max itemSize exceeded");

  double tReadStart = getTime();

  if (end - ptr != 0)
    memmove(b, ptr, end - ptr);

  end -= ptr - b;
  ptr = b;


  while (end < b + itemSize) {
    size_t n = fread((U8 *)end, b + BUFSIZE - end, 1, file);
    if (n < 1) {
      tRead += getTime() - tReadStart;
      if (n < 0 || ferror(file))
        throw Exception(strerror(errno));
      if (feof(file))
        throw EndOfStream();
      if (n == 0) { return 0; }
    }
    end += b + BUFSIZE - end;
  }

  if (itemSize * nItems > end - ptr)
    nItems = (end - ptr) / itemSize;

  tRead += getTime() - tReadStart;

  return nItems;
}
