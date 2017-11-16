#!/usr/bin/python2.7
import os, sys, shutil, platform, time
import cembed

OUTPUT = "bin/seraph"
EMBED_DIR = "src/embed"
TEMPSRC_DIR = ".tempsrc"
COMPILER = "gcc"
INCLUDE = [ "src/lib", TEMPSRC_DIR ]
SOURCE = [
  "src/*.c",
  "src/lib/fs/*.c",
  "src/lib/sera/*.c",
  "src/lib/glew/glew.c",
]

FLAGS = [ "-Wall", "-Wextra", "--std=c99", "-fno-strict-aliasing", "-Wno-unused-function", "-Wno-misleading-indentation", "-Wno-unused-parameter" ]
LINK = [ "m" ]
DEFINE = [ "GLEW_STATIC" ]
EXTRA = [  ]

if platform.system() == "Windows":
  sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
  OUTPUT += ".exe"
  LINK += [ "mingw32", "SDLmain", "SDL", "opengl32" ]
  FLAGS += [ "-mwindows" ]
  DEFINE += [ "SR_MODE_RGBA" ]

if platform.system() == "Linux":
  LINK += [ "SDLmain", "SDL", "GL" ]

if platform.system() == "Darwin":
  FLAGS += [ os.popen("sdl-config --cflags").read().strip() ]
  EXTRA += [ os.popen("sdl-config --libs").read().strip(),  "-framework OpenGL"  ]
  DEFINE += [ "SR_MODE_ARGB" ]


def fmt(fmt, dic):
  for k in dic:
    fmt = fmt.replace("{" + k + "}", str(dic[k]))
  return fmt


def clearup():
  if os.path.exists(TEMPSRC_DIR):
    shutil.rmtree(TEMPSRC_DIR)


def main():
  global FLAGS, SOURCE, LINK

  print "initing..."
  starttime = time.time()

  # Handle args
  build = "release" if "release" in sys.argv else "debug"
  verbose = "verbose" in sys.argv

  # Handle build type
  if build == "debug":
    FLAGS += [ "-g" ]
  else:
    FLAGS += [ "-O3" ]

  print "building (" + build + ")..."

  # Make sure there arn't any temp files left over from a previous build
  clearup()
  clearup()

  # Create directories
  os.makedirs(TEMPSRC_DIR)
  outdir = os.path.dirname(OUTPUT)
  if not os.path.exists(outdir):
    os.makedirs(outdir)

  # Create embedded-file header files
  for filename in os.listdir(EMBED_DIR):
    fullname = EMBED_DIR + "/" + filename
    res = cembed.process(fullname)
    open(TEMPSRC_DIR + "/" + cembed.safename(fullname) + ".h", "wb").write(res)


  # Build
  cmd = fmt(
    "{compiler} -o {output} {flags} {source} {include} {link} {define} " +
    "{extra}",
    {
      "compiler"  : COMPILER,
      "output"    : OUTPUT,
      "source"    : " ".join(SOURCE),
      "include"   : " ".join(map(lambda x:"-I" + x, INCLUDE)),
      "link"      : " ".join(map(lambda x:"-l" + x, LINK)),
      "define"    : " ".join(map(lambda x:"-D" + x, DEFINE)),
      "flags"     : " ".join(FLAGS),
      "extra"     : " ".join(EXTRA),
    })

  if verbose:
    print cmd

  print "compiling..."
  res = os.system(cmd)

  if build == "release":
    if os.path.isfile(os.path.dirname(OUTPUT)):
      print "stripping..."
      os.system("strip %s" % OUTPUT)

  print "clearing up..."
  clearup()
  clearup()

  if res == 0:
    print "done (%.2fs)" % (time.time() - starttime)
  else:
    print "done with errors"
  sys.exit(res)


if __name__ == "__main__":
  main()
