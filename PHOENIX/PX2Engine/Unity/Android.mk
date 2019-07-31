
ifndef USE_FREETYPE
USE_FREETYPE := 2.4.2
endif

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Unity

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/AppPlay/*cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/PlatformSDK/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%) \
google/protobuf/any.cc                     \
google/protobuf/any.pb.cc                  \
google/protobuf/arena.cc            \
google/protobuf/arenastring.cc                \
google/protobuf/descriptor.cc                  \
google/protobuf/descriptor.pb.cc            \
google/protobuf/descriptor_database.cc   \
google/protobuf/duration.pb.cc         \
google/protobuf/dynamic_message.cc                        \
google/protobuf/empty.pb.cc                   \
google/protobuf/extension_set.cc                 \
google/protobuf/extension_set_heavy.cc                 \
google/protobuf/field_mask.pb.cc                        \
google/protobuf/generated_message_reflection.cc                    \
google/protobuf/generated_message_util.cc              \
google/protobuf/map_field.cc                    \
google/protobuf/message.cc               \
google/protobuf/message_lite.cc					  \
google/protobuf/reflection_ops.cc					  \
google/protobuf/repeated_field.cc					  \
google/protobuf/service.cc					  \
google/protobuf/source_context.pb.cc					  \
google/protobuf/struct.pb.cc					  \
google/protobuf/text_format.cc					  \
google/protobuf/timestamp.pb.cc					  \
google/protobuf/type.pb.cc					  \
google/protobuf/unknown_field_set.cc					  \
google/protobuf/wire_format.cc					  \
google/protobuf/wire_format_lite.cc					  \
google/protobuf/wrappers.pb.cc					  \
google/protobuf/io/coded_stream.cc                \
google/protobuf/io/gzip_stream.cc                \
google/protobuf/io/printer.cc                \
google/protobuf/io/strtod.cc                \
google/protobuf/io/tokenizer.cc                \
google/protobuf/io/zero_copy_stream.cc \
google/protobuf/io/zero_copy_stream_impl.cc \
google/protobuf/io/zero_copy_stream_impl_lite.cc \
google/protobuf/stubs/atomicops_internals_x86_gcc.cc \
google/protobuf/stubs/atomicops_internals_x86_msvc.cc \
google/protobuf/stubs/bytestream.cc \
google/protobuf/stubs/common.cc \
google/protobuf/stubs/int128.cc \
google/protobuf/stubs/mathlimits.cc \
google/protobuf/stubs/once.cc \
google/protobuf/stubs/status.cc \
google/protobuf/stubs/statusor.cc \
google/protobuf/stubs/stringpiece.cc \
google/protobuf/stubs/stringprintf.cc \
google/protobuf/stubs/structurally_valid.cc \
google/protobuf/stubs/strutil.cc \
google/protobuf/stubs/substitute.cc               \
google/protobuf/stubs/time.cc               \
google/protobuf/util/internal/datapiece.cc\
google/protobuf/util/internal/default_value_objectwriter.cc\
google/protobuf/util/internal/error_listener.cc\
google/protobuf/util/internal/field_mask_utility.cc\
google/protobuf/util/internal/json_escaping.cc\
google/protobuf/util/internal/json_objectwriter.cc\
google/protobuf/util/internal/json_stream_parser.cc\
google/protobuf/util/internal/object_writer.cc\
google/protobuf/util/internal/proto_writer.cc\
google/protobuf/util/internal/protostream_objectsource.cc\
google/protobuf/util/internal/protostream_objectwriter.cc\
google/protobuf/util/internal/type_info.cc\
google/protobuf/util/internal/type_info_test_helper.cc\
google/protobuf/util/internal/utility.cc\
google/protobuf/util/field_comparator.cc\
google/protobuf/util/field_mask_util.cc\
google/protobuf/util/json_util.cc\
google/protobuf/util/message_differencer.cc\
google/protobuf/util/time_util.cc\
google/protobuf/util/type_resolver_util.cc\
ZLib/adler32.c \
ZLib/compress.c \
ZLib/crc32.c \
ZLib/deflate.c \
ZLib/gzclose.c \
ZLib/gzlib.c \
ZLib/gzread.c \
ZLib/gzwrite.c \
ZLib/infback.c \
ZLib/inffast.c \
ZLib/inflate.c \
ZLib/inftrees.c \
ZLib/trees.c \
ZLib/uncompr.c \
ZLib/zutil.c \
ZLib/minzip/ioapi.c \
ZLib/minzip/unzip.c \
ImageLibs/PNG/png.c \
ImageLibs/PNG/pngerror.c \
ImageLibs/PNG/pngget.c \
ImageLibs/PNG/pngmem.c \
ImageLibs/PNG/pngpread.c \
ImageLibs/PNG/pngread.c \
ImageLibs/PNG/pngrio.c \
ImageLibs/PNG/pngrtran.c \
ImageLibs/PNG/pngrutil.c \
ImageLibs/PNG/pngset.c \
ImageLibs/PNG/pngtrans.c \
ImageLibs/PNG/pngwio.c \
ImageLibs/PNG/pngwrite.c \
ImageLibs/PNG/pngwtran.c \
ImageLibs/PNG/pngwutil.c \
FreeType/src/base/ftbbox.c \
FreeType/src/base/ftbitmap.c \
FreeType/src/base/ftfstype.c \
FreeType/src/base/ftglyph.c \
FreeType/src/base/ftlcdfil.c \
FreeType/src/base/ftstroke.c \
FreeType/src/base/fttype1.c \
FreeType/src/base/ftxf86.c \
FreeType/src/base/ftbase.c \
FreeType/src/base/ftsystem.c \
FreeType/src/base/ftinit.c \
FreeType/src/base/ftgasp.c \
FreeType/src/raster/raster.c \
FreeType/src/sfnt/sfnt.c \
FreeType/src/smooth/smooth.c \
FreeType/src/autofit/autofit.c \
FreeType/src/truetype/truetype.c \
FreeType/src/cff/cff.c \
FreeType/src/psnames/psnames.c \
FreeType/src/pshinter/pshinter.c \
CURL/src/file.c \
CURL/src/timeval.c \
CURL/src/base64.c \
CURL/src/hostip.c \
CURL/src/progress.c \
CURL/src/formdata.c \
CURL/src/cookie.c \
CURL/src/http.c \
CURL/src/sendf.c \
CURL/src/ftp.c \
CURL/src/url.c \
CURL/src/dict.c \
CURL/src/speedcheck.c \
CURL/src/ssluse.c \
CURL/src/version.c \
CURL/src/getenv.c \
CURL/src/escape.c \
CURL/src/mprintf.c \
CURL/src/telnet.c \
CURL/src/netrc.c \
CURL/src/getinfo.c \
CURL/src/transfer.c \
CURL/src/strequal.c \
CURL/src/easy.c \
CURL/src/security.c \
CURL/src/curl_fnmatch.c \
CURL/src/fileinfo.c \
CURL/src/ftplistparser.c \
CURL/src/wildcard.c \
CURL/src/krb5.c \
CURL/src/memdebug.c \
CURL/src/http_chunks.c \
CURL/src/strtok.c \
CURL/src/connect.c \
CURL/src/llist.c \
CURL/src/hash.c \
CURL/src/multi.c \
CURL/src/content_encoding.c \
CURL/src/share.c \
CURL/src/http_digest.c \
CURL/src/md4.c \
CURL/src/md5.c \
CURL/src/http_negotiate.c \
CURL/src/inet_pton.c \
CURL/src/strtoofft.c \
CURL/src/strerror.c \
CURL/src/amigaos.c \
CURL/src/hostasyn.c \
CURL/src/hostip4.c \
CURL/src/hostip6.c \
CURL/src/hostsyn.c \
CURL/src/inet_ntop.c \
CURL/src/parsedate.c \
CURL/src/select.c \
CURL/src/gtls.c \
CURL/src/sslgen.c \
CURL/src/tftp.c \
CURL/src/splay.c \
CURL/src/strdup.c \
CURL/src/socks.c \
CURL/src/ssh.c \
CURL/src/nss.c \
CURL/src/qssl.c \
CURL/src/rawstr.c \
CURL/src/curl_addrinfo.c \
CURL/src/socks_gssapi.c \
CURL/src/socks_sspi.c \
CURL/src/curl_sspi.c \
CURL/src/slist.c \
CURL/src/nonblock.c \
CURL/src/curl_memrchr.c \
CURL/src/imap.c \
CURL/src/pop3.c \
CURL/src/smtp.c \
CURL/src/pingpong.c \
CURL/src/rtsp.c \
CURL/src/curl_threads.c \
CURL/src/warnless.c \
CURL/src/hmac.c \
CURL/src/polarssl.c \
CURL/src/polarssl_threadlock.c \
CURL/src/curl_rtmp.c \
CURL/src/curl_gethostname.c \
CURL/src/gopher.c \
CURL/src/axtls.c \
CURL/src/idn_win32.c \
CURL/src/http_negotiate_sspi.c \
CURL/src/cyassl.c \
CURL/src/http_proxy.c \
CURL/src/non-ascii.c \
CURL/src/asyn-ares.c \
CURL/src/asyn-thread.c \
CURL/src/curl_gssapi.c \
CURL/src/curl_ntlm.c \
CURL/src/curl_ntlm_wb.c \
CURL/src/curl_ntlm_core.c \
CURL/src/curl_ntlm_msgs.c \
CURL/src/curl_sasl.c \
CURL/src/curl_schannel.c \
CURL/src/curl_multibyte.c \
CURL/src/curl_darwinssl.c \
CURL/src/hostcheck.c \
CURL/src/bundles.c \
CURL/src/conncache.c \
CURL/src/pipeline.c \
CURL/src/dotdot.c \
CURL/src/x509asn1.c \
CURL/src/gskit.c \
CURL/src/http2.c \
Lua/lapi.c \
Lua/lauxlib.c \
Lua/lbaselib.c \
Lua/lcode.c \
Lua/ldblib.c \
Lua/ldebug.c \
Lua/ldo.c \
Lua/ldump.c \
Lua/lfunc.c \
Lua/lgc.c \
Lua/linit.c \
Lua/liolib.c \
Lua/llex.c \
Lua/lmathlib.c \
Lua/lmem.c \
Lua/loadlib.c \
Lua/lobject.c \
Lua/lopcodes.c \
Lua/loslib.c \
Lua/lparser.c \
Lua/lstate.c \
Lua/lstring.c \
Lua/lstrlib.c \
Lua/ltable.c \
Lua/ltablib.c \
Lua/ltm.c \
Lua/lundump.c \
Lua/lvm.c \
Lua/lzio.c \
Lua/print.c \
ToLua/tolua_event.c \
ToLua/tolua_is.c \
ToLua/tolua_map.c \
ToLua/tolua_push.c \
ToLua/tolua_to.c \
LuaPlus/LuaPlus.cpp \
LuaPlus/LuaPlusAddons.c \
SQLite/sqlite3.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(LOCAL_PATH)/ImageLibs/PNG/ \
$(LOCAL_PATH)/ZLib/ \
$(LOCAL_PATH)/ZLib/minzip/ \
$(LOCAL_PATH)/FreeType/builds/ \
$(LOCAL_PATH)/FreeType/include/ \
$(LOCAL_PATH)/Lua/ \
$(LOCAL_PATH)/ToLua/ \
$(LOCAL_PATH)/LuaPlus/ \
$(LOCAL_PATH)/CURL/config/android \
$(LOCAL_PATH)/CURL/src \
$(LOCAL_PATH)/CURL/ \
$(LOCAL_PATH)/SQLite/ \
$(PX2_ROOT)/PX2SDK/Include/ \
$(LOCAL_PATH)/AppPlay/ \
$(LOCAL_PATH)/PlatformSDK/ \
$(LOCAL_PATH)/SoundFMod/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_SHARED_LIBRARIES := fmod
#LOCAL_SHARED_LIBRARIES += vlc

# define the macros          
LOCAL_CFLAGS := -D__ANDROID__ -DHAVE_PTHREAD -DPX2_USE_FMOD -DLUAPLUS_EXCEPTIONS 
# -DPX2_USE_VLC
LOCAL_CFLAGS += -DIOAPI_NO_64
LOCAL_CFLAGS += -W -Wall
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += -DDARWIN_NO_CARBON
LOCAL_CFLAGS += -DFT2_BUILD_LIBRARY
LOCAL_CFLAGS += -O2

LOCAL_CPPFLAGS := -frtti -std=c++11 -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module, FMOD/android)
#$(call import-module, vlc)