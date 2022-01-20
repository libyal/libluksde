#!/usr/bin/env python
#
# Python-bindings volume type test script
#
# Copyright (C) 2013-2022, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import os
import random
import sys
import unittest

import pyluksde


class VolumeTypeTests(unittest.TestCase):
  """Tests the volume type."""

  def test_signal_abort(self):
    """Tests the signal_abort function."""
    luksde_volume = pyluksde.volume()

    luksde_volume.signal_abort()

  def test_open(self):
    """Tests the open function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    luksde_volume.open(test_source)

    with self.assertRaises(IOError):
      luksde_volume.open(test_source)

    luksde_volume.close()

    with self.assertRaises(TypeError):
      luksde_volume.open(None)

    with self.assertRaises(ValueError):
      luksde_volume.open(test_source, mode="w")

  def test_open_file_object(self):
    """Tests the open_file_object function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    with open(test_source, "rb") as file_object:

      luksde_volume.open_file_object(file_object)

      with self.assertRaises(IOError):
        luksde_volume.open_file_object(file_object)

      luksde_volume.close()

      with self.assertRaises(TypeError):
        luksde_volume.open_file_object(None)

      with self.assertRaises(ValueError):
        luksde_volume.open_file_object(file_object, mode="w")

  def test_close(self):
    """Tests the close function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    with self.assertRaises(IOError):
      luksde_volume.close()

  def test_open_close(self):
    """Tests the open and close functions."""
    test_source = unittest.source
    if not test_source:
      return

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    # Test open and close.
    luksde_volume.open(test_source)
    luksde_volume.close()

    # Test open and close a second time to validate clean up on close.
    luksde_volume.open(test_source)
    luksde_volume.close()

    if os.path.isfile(test_source):
      with open(test_source, "rb") as file_object:

        # Test open_file_object and close.
        luksde_volume.open_file_object(file_object)
        luksde_volume.close()

        # Test open_file_object and close a second time to validate clean up on close.
        luksde_volume.open_file_object(file_object)
        luksde_volume.close()

        # Test open_file_object and close and dereferencing file_object.
        luksde_volume.open_file_object(file_object)
        del file_object
        luksde_volume.close()

  def test_is_locked(self):
    """Tests the is_locked function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    luksde_volume = pyluksde.volume()

    luksde_volume.open(test_source)

    result = luksde_volume.is_locked()
    self.assertTrue(result)

    luksde_volume.close()

    if unittest.password:
      luksde_volume = pyluksde.volume()
      luksde_volume.set_password(unittest.password)

      luksde_volume.open(test_source)

      result = luksde_volume.is_locked()
      self.assertFalse(result)

      luksde_volume.close()

  def test_read_buffer(self):
    """Tests the read_buffer function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    luksde_volume.open(test_source)

    size = luksde_volume.get_size()

    if size < 4096:
      # Test read without maximum size.
      luksde_volume.seek_offset(0, os.SEEK_SET)

      data = luksde_volume.read_buffer()

      self.assertIsNotNone(data)
      self.assertEqual(len(data), size)

    # Test read with maximum size.
    luksde_volume.seek_offset(0, os.SEEK_SET)

    data = luksde_volume.read_buffer(size=4096)

    self.assertIsNotNone(data)
    self.assertEqual(len(data), min(size, 4096))

    if size > 8:
      luksde_volume.seek_offset(-8, os.SEEK_END)

      # Read buffer on size boundary.
      data = luksde_volume.read_buffer(size=4096)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), 8)

      # Read buffer beyond size boundary.
      data = luksde_volume.read_buffer(size=4096)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), 0)

    # Stress test read buffer.
    luksde_volume.seek_offset(0, os.SEEK_SET)

    remaining_size = size

    for _ in range(1024):
      read_size = int(random.random() * 4096)

      data = luksde_volume.read_buffer(size=read_size)

      self.assertIsNotNone(data)

      data_size = len(data)

      if read_size > remaining_size:
        read_size = remaining_size

      self.assertEqual(data_size, read_size)

      remaining_size -= data_size

      if not remaining_size:
        luksde_volume.seek_offset(0, os.SEEK_SET)

        remaining_size = size

    with self.assertRaises(ValueError):
      luksde_volume.read_buffer(size=-1)

    luksde_volume.close()

    # Test the read without open.
    with self.assertRaises(IOError):
      luksde_volume.read_buffer(size=4096)

  def test_read_buffer_file_object(self):
    """Tests the read_buffer function on a file-like object."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    with open(test_source, "rb") as file_object:
      luksde_volume.open_file_object(file_object)

      size = luksde_volume.get_size()

      # Test normal read.
      data = luksde_volume.read_buffer(size=4096)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), min(size, 4096))

      luksde_volume.close()

  def test_read_buffer_at_offset(self):
    """Tests the read_buffer_at_offset function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    luksde_volume.open(test_source)

    size = luksde_volume.get_size()

    # Test normal read.
    data = luksde_volume.read_buffer_at_offset(4096, 0)

    self.assertIsNotNone(data)
    self.assertEqual(len(data), min(size, 4096))

    if size > 8:
      # Read buffer on size boundary.
      data = luksde_volume.read_buffer_at_offset(4096, size - 8)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), 8)

      # Read buffer beyond size boundary.
      data = luksde_volume.read_buffer_at_offset(4096, size + 8)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), 0)

    # Stress test read buffer.
    for _ in range(1024):
      random_number = random.random()

      media_offset = int(random_number * size)
      read_size = int(random_number * 4096)

      data = luksde_volume.read_buffer_at_offset(read_size, media_offset)

      self.assertIsNotNone(data)

      remaining_size = size - media_offset

      data_size = len(data)

      if read_size > remaining_size:
        read_size = remaining_size

      self.assertEqual(data_size, read_size)

      remaining_size -= data_size

      if not remaining_size:
        luksde_volume.seek_offset(0, os.SEEK_SET)

    with self.assertRaises(ValueError):
      luksde_volume.read_buffer_at_offset(-1, 0)

    with self.assertRaises(ValueError):
      luksde_volume.read_buffer_at_offset(4096, -1)

    luksde_volume.close()

    # Test the read without open.
    with self.assertRaises(IOError):
      luksde_volume.read_buffer_at_offset(4096, 0)

  def test_seek_offset(self):
    """Tests the seek_offset function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    luksde_volume.open(test_source)

    size = luksde_volume.get_size()

    luksde_volume.seek_offset(16, os.SEEK_SET)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, 16)

    luksde_volume.seek_offset(16, os.SEEK_CUR)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, 32)

    luksde_volume.seek_offset(-16, os.SEEK_CUR)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, 16)

    if size > 16:
      luksde_volume.seek_offset(-16, os.SEEK_END)

      offset = luksde_volume.get_offset()
      self.assertEqual(offset, size - 16)

    luksde_volume.seek_offset(16, os.SEEK_END)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, size + 16)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(-1, os.SEEK_SET)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(-32 - size, os.SEEK_CUR)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(-32 - size, os.SEEK_END)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(0, -1)

    luksde_volume.close()

    # Test the seek without open.
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(16, os.SEEK_SET)

  def test_get_offset(self):
    """Tests the get_offset function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    luksde_volume.open(test_source)

    offset = luksde_volume.get_offset()
    self.assertIsNotNone(offset)

    luksde_volume.close()

  def test_get_size(self):
    """Tests the get_size function and size property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    luksde_volume = pyluksde.volume()
    if unittest.password:
      luksde_volume.set_password(unittest.password)

    luksde_volume.open(test_source)

    size = luksde_volume.get_size()
    self.assertIsNotNone(size)

    self.assertIsNotNone(luksde_volume.size)

    luksde_volume.close()


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "-p", "--password", dest="password", action="store", default=None,
      type=str, help="password to unlock the source file.")

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "password", options.password)
  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
