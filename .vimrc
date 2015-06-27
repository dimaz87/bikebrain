set wildignore=*.o,*.html,*.md5,*.exe,*.dll,*.bak,*.png,*.jpg,*.orig,*.swp,*.swo,*.pyc,*/.git/*,*/.hg/*,*/.bzr/*,*/doxygen/*,*/CMakeFiles/*,ui/*,etc/*,*/cxxtest/*,*/v8/*,*/toolchain/*,*/thirdparty/*
let g:exclude_from_search=['stingray_wrap.cpp']

set path+=stingraykit
let g:include_directories = [ 'stingraykit' ]

let g:cpp_plugin.whitespacesCountAroundImports = 1
let g:cpp_plugin.indexer.builder.autoBuild = 1
call g:cpp_plugin.indexer.builder.exclude([ '*CMakeFiles*', '*doxygen*', '*native/v8*', './build/test-runner.cpp', './etc*', './stingray/platform/celestial/af9013/*', './stingray/platform/cxd2820/driver/*', './stingray/platform/emma3/modules/*', './stingray/platform/emma3/tuners/nec_dvbs2_defs.h', './stingray/platform/np6/registers.h', '*.tab.h', '*.yy.h', '*.tab.cpp', '*.yy.cpp', './stingray/test/cxxtest/*', './stingray/thirdparty/coredumper/elfcore.c', './stingray/thirdparty/coredumper/linux_syscall_support.h', './stingray/toolkit/bind.h', './ui.argile/bin/argile/pyparsing.py', './ui.argile/native/argile/RTree.h', './ui.argile/native/argile/backend/celestial/blit_cmd_reg_def.h', './ui.argile/native/argile/backend/celestial/blit_hw.h', './ui.argile/native/argile/backend/celestial/orion_gfxobj.h', './ui.argile/native/argile/backend/software/dlmalloc.c', './ui.argile/native/argile/lowlevel/PixelsArray.h', './ui/*', './ui/qtsoap/qtsoap.cpp', './ui/qtsoap/qtsoap.h', './ui.argile/native/argile/runtime/qml/swig*', './thirdparty/*', './toolchain/*' ])
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_SIMPLE_EXCEPTION[ \t]*\(([A-Za-z0-9_]*)[ \t]*,[ \t]*"[^"]*"[ \t]*\)/\1/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_PTR[ \t]*\(([A-Za-z0-9_]*)\)/\1Ptr/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_PTR[ \t]*\(([A-Za-z0-9_]*)\)/\1WeakPtr/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_PTR[ \t]*\(([A-Za-z0-9_]*)\)/\1ConstPtr/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_PTR[ \t]*\(([A-Za-z0-9_]*)\)/\1ConstWeakPtr/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_ENUMERATOR[ \t]*\(([A-Za-z0-9_]*)\)/\1Enumerator/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_ENUMERATOR[ \t]*\(([A-Za-z0-9_]*)\)/\1EnumeratorPtr/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_ENUMERABLE[ \t]*\(([A-Za-z0-9_]*)\)/\1Enumerator/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_ENUMERABLE[ \t]*\(([A-Za-z0-9_]*)\)/\1Enumerable/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_ENUMERABLE[ \t]*\(([A-Za-z0-9_]*)\)/\1EnumeratorPtr/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_ENUMERABLE[ \t]*\(([A-Za-z0-9_]*)\)/\1EnumerablePtr/s/')
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*STINGRAYKIT_DECLARE_COMPARER[ \t]*\(([A-Za-z0-9_]*)\)/\1ComparerPtr/s/')

let stingraykit = CTagsFrameworkInfo()
call stingraykit.addImports('stingraykit/function/bind.h', [ 'bind' ])
call g:cpp_plugin.indexer.registerFramework(stingraykit)

nmap <F1> yyjp>>^dWis_logger.Info() << "<ESC>A";<ESC>:s/\((\<Bar>, \)\([A-Za-z0-9_:]*\*\?\<Bar>const [A-Za-z0-9:_]*&\) \([A-Za-z0-9_]*\)\ze[,)]/\1" << \3 << "/ge<CR>:noh<CR>

if exists('g:c_std_includes') && exists('g:cpp_std_includes') && exists('g:platform_includes')
	let g:include_priorities = [ 'bikebrain/.*', 'stingraykit/.*', g:platform_includes, g:cpp_std_includes, g:c_std_includes ]
end

function! GetCppNamespaceFromPath(path)
	let res = []
	if len(a:path) > 0 && a:path[0] == 'bikebrain'
		call add(res, 'bikebrain')
	endif
	if (len(a:path) > 2 && a:path[1] == 'platform')
		call add(res, a:path[2])
	endif
	return res
endf

if file_readable('.custom_vimrc')
	source .custom_vimrc
end

