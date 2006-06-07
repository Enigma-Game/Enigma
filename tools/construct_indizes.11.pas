
program construct_indizes;

{ Transforms a tab-separated level-table into index-files and   }
{ moves the necessary files into newly created directories,     }
{ thereby removing CRs and adding a line ending at the fileend. }

{ Syntax: construct_indizes < leveldata.dat }

{ Options:
   -n   --nofiles   : Don't move or rename files
   -f   --flat      : Flat mode: simply transform the given file, induces -n
   -r   --ratings   : Use ratings, don't create index_*_ratings
   -b   --noratings : ('blank') Don't create index_*_ratings
   -m   --rename    : Rename lua/xml-files
   -on  --oldnames  : Use old filenames
   -nn  --newnames  : Use new filenames
   -h   --help  -?  : Help }

const
  Separator = #9;  { e.g. ';' or #9 = tab;
                     beware: the separator should not appear in the datafields! }
  NrColumns = 14;
  Columntitles : array[1..NrColumns] of AnsiString
    = ('oldname', 'title', 'author', 'indexname', 'easymode', 'par_time', 'par_time_normal',
       'par_time_easy', 'revision', 'int', 'dex', 'pat', 'kno', 'spe');
  ColumntitlesInIndex : array[1..NrColumns] of AnsiString
    = ('file', 'name', 'author', 'indexname', 'easymode', 'par_time', 'par_time_normal',
       'par_time_easy', 'revision', 'int', 'dex', 'pat', 'kno', 'spe');  
  Columnquotation : array[1..NrColumns] of AnsiString
    = ('', '"', '"', '', '', '', '', '', '', '', '', '', '', '');
  MaxDirectories = 142;
  MaxLevels = 10000;
  MaxIndizes = 100;
  MaxComments = 5;
  
  filename_NotASCII = 'not_ascii.txt';
  filename_NoFile = 'level_not_found.txt';
  filename_NoPng = 'png_not_found.txt';
  filename_Debug = 'debug.txt';
  filename_IndexLua = 'index.lua.masked';
  filename_Netplay1 = 'index_netplay.txt';
  filename_Netplay2 = 'index_netplay_ratings.txt';
  
  option_movefiles : boolean = TRUE;
  option_flatmode : boolean = FALSE;
  option_use_ratings : boolean = FALSE;
  option_create_ratingsindex : boolean = TRUE;
  option_new_filenames : boolean = FALSE;
  option_rename : boolean = FALSE;

  col_directory = 'directory';
  col_oldname = 'oldname';
  col_newname = 'newname';
  col_indexname = 'indexname';
  col_revision = 'revision';
  col_comment = 'title';
  col_pack = 'nPack';
  col_pack_nr = 'Nr';
  col_oldpack = 'oPack2';
  col_oldpack_nr = 'oNr';
  col_netplay = 'Np';
  col_netplay_nr = 'Np';
  col_tutorial = 'Tut';
  col_tutorial_nr = 'Tut';

type
  TypIndex = object
	       name, file_rated, file_unrated : AnsiString;
	       rated, unrated : array[0..MaxLevels] of AnsiString;
               comment : array[0..MaxLevels, 1..MaxComments] of AnsiString;
               empty : longint;
               procedure Init;
	       procedure Save;
	       procedure AddLevel(r, ur : AnsiString; j : longint);
               procedure AddComment(comm : AnsiString; j : longint);
	     end;


var  { global variables }
  f_notascii, f_nofile, f_nopng, f_debug : text;  { several files for output }
  f_indexlua : text;
  cr : longint;  { counts CR's }
  Header : AnsiString;
  Columns, Count : longint;
  directorylist : array[1..MaxDirectories] of AnsiString;
  lastentry : 1..MaxDirectories;
  netplay, tutorial : TypIndex;
  index : array[1..MaxIndizes] of TypIndex;

procedure closeandhalt;
begin
  {$I-}
  if option_movefiles then begin
    close(f_notascii);
    close(f_nofile);
    close(f_nopng);
  end;
  close(f_debug);
  close(f_indexlua);
  {$I+}
  halt;
end;

function tostring(x : longint) : AnsiString;
var st : AnsiString; begin str(x,st); tostring := st; end;
function toint(st : AnsiString) : longint;
var x : longint; c : word;
begin val(st,x,c); if c <> 0 then x := 0; toint := x; end;

procedure critical(st : AnsiString);
begin
  writeln(stderr, 'Critical error: ' + st);
  writeln(stderr, 'Exiting now.');
  writeln(f_debug, 'Critical error: ' + st);
  writeln(f_debug, 'Exiting now.');
  closeandhalt;  
end;

procedure TypIndex.Save;
var f : text; j, k : longint;
begin
  assign(f, file_unrated);
  rewrite(f);
  for j := 0 to empty - 1 do begin
    for k := 1 to MaxComments do
      if comment[j,k] <> '' then
	writeln(f, comment[j,k]);
    if unrated[j] <> '' then
      writeln(f, unrated[j]);
  end;
  close(f);
  if option_create_ratingsindex then begin
    assign(f, file_rated);
    rewrite(f);
    for j := 0 to empty - 1 do begin
      for k := 1 to MaxComments do
	if comment[j,k] <> '' then
	  writeln(f, comment[j,k]);      
      if rated[j] <> '' then
        writeln(f, rated[j]);
    end;
    close(f);
  end;
end;
procedure TypIndex.Init;
var j, k : longint;
begin
  for j := 0 to MaxLevels do begin
    rated[j] := '';
    unrated[j] := '';
    for k := 1 to MaxComments do
      comment[j,k] := '';
  end;
  empty := 1;
  name := '';
  file_rated := 'index_dummy.txt';
  file_unrated := 'index_dummy_ratings.txt';
end;
procedure TypIndex.AddLevel(r, ur : AnsiString; j : longint);
begin
  writeln(f_debug, 'Trying to add a level as number ' + tostring(j)
	  + ' to index ' + name + '...');
  if empty > MaxLevels then
    critical('Too much levels for index ' + name + '.');
  if (rated[j] = '') and (unrated[j] = '') then begin
    rated[j] := r;
    unrated[j] := ur;
    if j + 1 > empty then
      empty := j + 1;
  end else if (rated[j] = '') or (unrated[j] = '') then begin
    critical('Indexrecords became inconsistent at line ' + tostring(count)
	     + '. Very strange, indeed.');
  end else begin
    rated[empty] := r;
    unrated[empty] := ur;
    inc(empty);
  end;
  if empty <= MaxLevels then
    if (rated[empty] <> '') or (unrated[empty] <> '') then
      critical('Addlevel failed for position ' + tostring(j) + ' in pack ' + name + '.');
end;
procedure TypIndex.AddComment(comm: AnsiString; j : longint);
var found : boolean; k : longint;
begin
  writeln(f_debug, 'Trying to add a comment to number ' + tostring(j)
	  + ' of index ' + name + '...');
  found := false;
  for k := 1 to MaxComments do
    if (not found) and (comment[j,k] = '') then begin
      comment[j,k] := comm;
      found := true;
    end;
  if not found then begin
    writeln(stderr, 'WARNING: Too many comments for position ' + tostring(j) +
	    ' in index ' + name + '!');
    writeln(f_debug, 'WARNING: Too many comments for position ' + tostring(j) +
	    ' in index ' + name + '!');
  end;
end;

function ExtractColumn(st : AnsiString; colnr : longint) : AnsiString;
var searched : AnsiString; j : longint;
begin
  searched := st;
  for j := 1 to colnr - 1 do
    searched := copy(searched, pos(Separator, searched) + 1, length(searched));  
  searched := copy(searched, 1, pos(Separator, searched) - 1);
  ExtractColumn := searched;
end;
function ExtractByTitle(st : AnsiString; name : AnsiString) : AnsiString;
var j, k : longint; st2 : AnsiString;
begin
  k := 0;
  if pos(name, Header) = 0 then
    critical('Header incomplete, I''m missing the identifier ' + name + '.');
  for j := 1 to Columns do
    if (k = 0) and (ExtractColumn(Header, j) = name) then
      k := j;
  st2 := ExtractColumn(st, k);
  if (st2 = '') and (name = col_indexname) then
    st2 := ExtractByTitle(st, col_oldname);
  if name = col_oldpack then
    st2 := 'old_' + st2;    
  ExtractByTitle := st2;
end;

function NewNameWithRevision(st : AnsiString) : AnsiString;
var nn, rev : AnsiString;
begin
  nn := ExtractByTitle(st, col_newname);
  if (pos('_', nn) = 0) and (ExtractByTitle(st, col_directory) <> 'lib') then begin
    rev := ExtractByTitle(st, col_revision);
    if rev = '' then rev := '1';
    nn := nn + '_' + rev;
  end;
  NewNameWithRevision := nn;
end;

function ConvertLine(st, destdir : AnsiString; ratings : boolean) : AnsiString;
var newst, filename : AnsiString; {j,} k, totalcols : longint;
    {colnr : array[1..NrColumns] of longint; }
begin
  { First, we analyse the Header. Yes, you're right: We analyse the Header
    again and again, each time we enter this procedure, and the result will
    always be the same. Not very efficient, but who cares? }
  {j := 1;
  for k := 1 to NrColumns do colnr[k] := 0;  
  repeat
    newst := ExtractColumn(Header, j);
    for k := 1 to NrColumns do
      if newst = Columntitles[k] then colnr[k] := j;
    inc(j);
  until (colnr[1] <> 0) and (newst = '');}
  { Now, as we know where to search for, we can construct our converted line: }
  { The filename-entry is special, it contains destdir: }
  newst := '';
  if option_new_filenames xor option_rename then    
    filename := NewNameWithRevision(st)  {ExtractByTitle(st, col_newname)}
  else
    filename := ExtractByTitle(st, col_oldname);
  if filename <> '' then begin
    newst := '{  ' + newst + ColumntitlesInIndex[1] + '=' + destdir + '/'
      + Columnquotation[1] + filename + Columnquotation[1] + '  ';
    { Now we look at the other entries: }
    if ratings or option_use_ratings then
      totalcols := NrColumns else totalcols := NrColumns - 5;
    for k := 2 to totalcols do
      if (ExtractByTitle(st, Columntitles[k]) <> '') then begin
        newst := newst + ColumntitlesInIndex[k] + '=' + Columnquotation[k]
                       + ExtractByTitle(st, Columntitles[k])
                       + Columnquotation[k] + '  ';
      end;
    newst := newst + '}';
    if newst = '{  }' then newst := '';
  end else
    newst := '';
  ConvertLine := newst;
end;

function SearchPack(pack : AnsiString) : longint;
var j, k, nr : longint; found : boolean;
begin
  nr := -1;
  if not ((pack = '') or (pack = '0')) then begin
    found := false;
    for j := 1 to MaxIndizes do
      if (not found) and (index[j].name = pack) then begin
        found := true;
	nr := j;
      end;
    if not found then begin
      k := 0;
      for j := 1 to MaxIndizes do
        if (k = 0) and (index[j].name = '') then k := j;
      writeln(stderr, 'Creating index ' + pack + ' as number ' + tostring(k) + '.');
      writeln(f_debug, 'Creating index ' + pack + ' as number ' + tostring(k) + '.');
      index[k].Init;
      index[k].name := pack;
      index[k].file_unrated := 'index_' + pack + '.txt';
      index[k].file_rated := 'index_' + pack + '_ratings.txt';
      nr := k;
    end;
  end;
  SearchPack := nr;
end;

procedure SearchAndAddCommentToPack(pack, comm : AnsiString; pos : longint);
var j : longint;
begin
  j := SearchPack(pack);
  if j <> -1 then
    index[j].AddComment(comm, pos);
end;

procedure SearchAndAddToPack(pack, rated, unrated : AnsiString; pos : longint);
var j : longint;
begin
  j := SearchPack(pack);
  if j <> -1 then
    index[j].AddLevel(rated, unrated, pos);
end;

procedure AddLevelToIndizes(st : AnsiString);
var destdir, rated, unrated, pack, oldpack, net, tut : AnsiString;
  pack_nr, oldpack_nr, net_nr, tut_nr : longint;
begin
  { Each level can exist in four different Indizes:
      1. Its enigma-, clones- or puretutorial-pack (real packs)
      2. Its old enigma-pack (purely virtual)
      3. netplay-pack (purely virtual)
      4. tutorial-pack (purely virtual)
    Position 1:  col_pack / col_pack_nr
    Position 2:  col_oldpack / col_oldpack_nr
    Position 3:  col_netplay / col_netplay_nr
    Position 4:  col_tutorial / col_tutorial_nr
  }
  destdir := ExtractByTitle(st, col_directory);
  rated := ConvertLine(st, destdir, TRUE);
  unrated := ConvertLine(st, destdir, FALSE);
  pack := ExtractByTitle(st, col_pack);
  pack_nr := toint(ExtractByTitle(st, col_pack_nr));
  oldpack := ExtractByTitle(st, col_oldpack);
  oldpack_nr := toint(ExtractByTitle(st, col_oldpack_nr));
  net := ExtractByTitle(st, col_netplay);
  net_nr := toint(ExtractByTitle(st, col_netplay_nr));
  tut := ExtractByTitle(st, col_tutorial);
  tut_nr := toint(ExtractByTitle(st, col_tutorial_nr));
  writeln(stderr, 'Line ', count, ' appending to ', pack, ' and ', oldpack, '.');	  
  writeln(f_debug, 'Line ', count, ' appending to ', pack, ' and ', oldpack, '.');
  { First, search for pack and oldpack. }
  if pack <> 'tutorial' then
    SearchAndAddToPack(pack, rated, unrated, pack_nr);
  if (oldpack <> 'tutorial') and (oldpack <> pack) then
    SearchAndAddToPack(oldpack, rated, unrated, oldpack_nr);
  { Then add to netplay and tutorial. }
  if (net <> '') and (net <> '0') then
    netplay.AddLevel(rated, unrated, net_nr);
  if (tut <> '') and (tut <> '0') then
    tutorial.AddLevel(rated, unrated, tut_nr);
end;


procedure MoveLevelfile(destdir, oldname, newname : AnsiString);
var fulloldname, fullnewname : AnsiString; fin, fout : file of byte;
  ioerr : longint; b : byte;
begin
  { First, find the right filename: lua or xml? }
  fulloldname := oldname + '.lua';
  fullnewname := newname + '.lua';
  {$I-}   { deactivate automatical I/O-checking }
  assign(fin, fulloldname);
  reset(fin);
  ioerr := IOResult;
  {$I+}
  if ioerr = 2 then begin { not found, maybe xml? }
    fulloldname := oldname + '.xml';
    fullnewname := newname + '.xml';
    {$I-}
    assign(fin, fulloldname);
    reset(fin);
    ioerr := IOResult;
    {$I+}
  end;
  if ioerr = 2 then  { No xml also? Append to error-file! }
    writeln(f_nofile, 'Levelfile not found: ', oldname, ' for directory ', destdir, '.')
  else if ioerr <> 0 then
    critical('I/O-Error Nr ' + tostring(ioerr) + ' while trying to open some file like '
	     + oldname + '.lua/xml.')
  else begin
    { Okay, at this point fin holds the original file, fulloldname and fullnewname }
    { ehm... well... the full old name and the full new name... }  
    {$I-}
    assign(fout, destdir + '/' + fullnewname);
    rewrite(fout);
    ioerr := IOResult;
    {$I+}
    if ioerr <> 0 then begin
      close(fin);
      critical('I/O-Error Nr ' + tostring(ioerr) +
	      ' while trying to write to file ' + destdir + '/' + fullnewname);
    end;
    while not eof(fin) do begin
      read(fin, b);
      if (b = 9) or (b = 10) or ((b >= 23) and (b <= 126)) then
        write(fout, b)  { write ASCII }
      else if (b = 13) then
        inc(cr)  { delete CRs, but count their total }
      else begin
        writeln(f_notascii, 'Not-ASCII Nr. ', b, ' (', chr(b), ') found in file ',
  	      fulloldname, '.');
        write(fout, b);  { warn, but use same symbols }
      end;
    end;
    write(fout, 10); { end in newline }
    close(fout);
    close(fin);	 
    erase(fin);
  end;
end;

procedure MovePng(destdir, oldname, newname : AnsiString);
var fulloldname : AnsiString; fin, fout : file of byte;
  ioerr : longint; b : byte;
begin
  fulloldname := oldname + '.png';
  {fullnewname := newname + '.png';}
  {$I-}   { deactivate automatical I/O-checking }
  assign(fin, fulloldname);
  reset(fin);
  ioerr := IOResult;
  {$I+}
  if ioerr = 2 then begin { not found, maybe an auto-thumbnail? }
    fulloldname := '1_1_' + oldname + '.png';
    {fullnewname := '1_1_' + newname + '.png';}
    {$I-}
    assign(fin, fulloldname);
    reset(fin);
    ioerr := IOResult;
    {$I+}
  end;
  if ioerr = 2 then begin { not found, maybe an auto-thumbnail from the alle-directory }
    fulloldname := '1_1_alle_' + oldname + '.png';
    {fullnewname := '1_1_alle_' + newname + '.png';}
    {$I-}
    assign(fin, fulloldname);
    reset(fin);
    ioerr := IOResult;
    {$I+}
  end;
  if ioerr = 2 then  { No png nowhere? Append to error-file! }    
    writeln(f_nopng, 'Thumbnail not found: ', oldname, ' for directory ', destdir, '.')
  else if ioerr <> 0 then begin
    critical('I/O-Error Nr ' + tostring(ioerr) + ' while trying to open some file like '
			+ oldname + '.png.');
  end else begin
    { Okay, at this point fin holds the original file. }  
    {$I-}
    assign(fout, destdir + '/' + newname + '.png');
    rewrite(fout);
    ioerr := IOResult;
    {$I+}
    if ioerr <> 0 then begin
      close(fin);
      critical('I/O-Error Nr ' + tostring(ioerr) +
	       ' while trying to write to file ' + destdir + '/' + newname + '.png');
    end;
    while not eof(fin) do begin
      read(fin, b);
      write(fout, b);  { Just copy... }
    end;
    close(fout);
    close(fin);	 
    erase(fin);
  end;
end;

procedure AnalyzeHeader;
begin
  { First, we search for the headline, which includes the word "filename": }    
  repeat
    readln(input, Header);
    inc(count);
  until (pos(Columntitles[1], Header) <> 0) or eof(input);
  if eof(input) then
    critical('Header not found!');
  Columns := length(Header);  { Yes, I know, I know... }  
  writeln(f_debug, 'AnalyzeHeader: ');
  writeln(f_debug, Header);
end;

procedure CreateDirectory(destdir : AnsiString);
var directoryfound : boolean; p : longint;
begin
  { Here we have a small problem: If we try to make a directory a second }
  { time, we get a runtime error 5 back, even though using $I-. So we    }
  { use some array to save the names of already created directories.     }
  { If we create a new directory, we should also add a corresponding     }
  { line in index.lua.                                                   }
  directoryfound := false;
  for p := 1 to MaxDirectories do
    if destdir = directorylist[p] then directoryfound := true;
  if not directoryfound then begin    
    writeln(stderr, 'Making Directory ', destdir, '...');
    writeln(f_debug, 'Making Directory ', destdir, '...');
    mkdir(destdir);
    directorylist[lastentry] := destdir;
    inc(lastentry);
    if lastentry >= MaxDirectories then begin
      writeln(stderr,
	'Please increase my MaxDirectories-number, this''s not enough. Exiting now.');
      writeln(f_debug,
	'Please increase my MaxDirectories-number, this''s not enough. Exiting now.');
      closeandhalt;
    end;
  end;
end;

procedure Init;
var p : longint;
begin
  write(stderr, 'Initialising...');
  { "Count" counts the line-number for debugging-reasons. }
  count := 0;
  { cr counts, ... well... CR's... }
  cr := 0;
  { We'll explain directorylist later... just initialize it here. }
  for p := 1 to MaxDirectories do directorylist[p] := '';
  lastentry := 1;
  { Open several errorfiles: }
  assign(f_debug, filename_Debug);
  assign(f_indexlua, filename_IndexLua);
  rewrite(f_debug);
  rewrite(f_indexlua);
  if option_movefiles then begin
    assign(f_notascii, filename_NotASCII);
    assign(f_nofile, filename_NoFile);
    assign(f_nopng, filename_NoPng);
    rewrite(f_notascii);
    rewrite(f_nofile);
    rewrite(f_nopng); 
  end;
  { Initialise packs. }
  for p := 1 to MaxIndizes do
    index[p].Init;
  netplay.Init;
  netplay.name := 'netplay';
  netplay.file_unrated := 'index_netplay.txt';
  netplay.file_rated := 'index_netplay_ratings.txt';
  tutorial.Init;
  tutorial.name := 'tutorial';
  tutorial.file_unrated := 'tutorial/index_tutorial.txt';
  tutorial.file_rated := 'tutorial/index_tutorial_ratings.txt';
  if not option_flatmode then
    CreateDirectory('tutorial');
  writeln(stderr, ' ready, go!');
  writeln(f_debug, 'Initialisation completed.');
end;

procedure WriteIndexLuaLine(i : TypIndex);
begin
  writeln(f_indexlua,'enigma.AddLevelPack("levels/', i.file_unrated,'", "',i.name,'")');
  if option_create_ratingsindex then
    writeln(f_indexlua,'--enigma.AddLevelPack("levels/', i.file_rated,'", "',i.name, '")');
end;
procedure WriteIndexLua;
var j : longint;
begin
  WriteIndexLuaLine(tutorial);
  for j := 1 to MaxIndizes do
    if index[j].name <> '' then
      WriteIndexLuaLine(index[j]);
  WriteIndexLuaLine(netplay);
end;

procedure WorkAboutInputString(st : AnsiString);
var  destdir, oldname, newname, comm, pack : AnsiString; pos : longint;
begin
  { Extract destfile, destdir and levelname from input AnsiString: }
  destdir := ExtractByTitle(st, col_directory);
  if destdir = '' then destdir := 'unassigned';
  { nowhere: Ignore. comment: Verbal. }
  if destdir = 'nowhere' then begin
  end else if destdir = 'comment' then begin
    { Add the comment located in col_comment verbally into (real) level index. }
    pack := ExtractByTitle(st, col_pack);
    pos := toint(ExtractByTitle(st, col_pack_nr));
    comm := ExtractByTitle(st, col_comment);
    SearchAndAddCommentToPack(pack, comm, pos);
  end else begin
    { Extract old and new filenames (that's complicated, bwaha!). }
    if option_new_filenames then begin
      oldname := NewNameWithRevision(st); {ExtractByTitle(st, col_newname);}
      newname := ExtractByTitle(st, col_oldname);
    end else begin
      oldname := ExtractByTitle(st, col_oldname);
      newname := NewNameWithRevision(st); {ExtractByTitle(st, col_newname);}
    end;
    if not option_rename then
      newname := oldname;
    { Make the directory, if not existing. }
    CreateDirectory(destdir);
    { Move the lua/xml-file and the png, if neccessary, rename: }
    if option_movefiles then begin
      writeln(f_debug, 'Trying to move file ', oldname, ' to ', destdir, ' / ', newname);
      MoveLevelfile(destdir, oldname, newname);
      MovePng(destdir, oldname, newname);
    end;
    { Now append to the indices: }
    writeln(f_debug, 'Trying to append to indices...');
    AddLevelToIndizes(st);
  end;
end;

procedure DoParameter(st : AnsiString);
begin
  st := Lowercase(st);
  if (st = '--help') or (st = '-h') or (st = '-?') or (st = '?') then begin
    writeln(stderr);
    writeln(stderr, 'Usage: ');
    writeln(stderr, '  ./construct_indizes.9 [options] < leveldaten.txt');
    writeln(stderr, 'Constructs level indices and directories from a single');
    writeln(stderr, 'tab-separated and sorted file-table, and a working');
    writeln(stderr, 'directory full of level files and thumbnails.');
    writeln(stderr, '-n, --nofiles : Suppress moving files. The indexfiles');
    writeln(stderr, '    will still use the calculated path and will be');
    writeln(stderr, '    created in the new directories.');
    writeln(stderr, '-f, --flat : Simply transform the given table to stdout.');
    writeln(stderr, '-r, --ratings : Use ratings, don''t create index_*_ratings.');
    writeln(stderr, '-b, --noratings : Don''t create index_*_ratings.');
    writeln(stderr, '-on, --oldnames : Use old filename-column (default).');
    writeln(stderr, '-nn, --newnames : Use new filename-column.');
    writeln(stderr, '-m, --rename : Rename from old to new (-nn: vice versa).');
    writeln(stderr, '-h, --help, -?, ? : This help.');
    writeln(stderr);
    halt;
  end else if (st = '--nofiles') or (st = '-n') then begin
    option_movefiles := FALSE;
  end else if (st = '-f') or (st = '--flat') then begin
    option_movefiles := FALSE;
    option_flatmode := TRUE;
  end else if (st = '-r') or (st = '--ratings') then begin
    option_use_ratings := TRUE;
    option_create_ratingsindex := FALSE;
  end else if (st = '-b') or (st = '--noratings') then begin
    option_create_ratingsindex := FALSE;
  end else if (st = '-on') or (st = '--oldnames') then begin
    option_new_filenames := FALSE;
  end else if (st = '-nn') or (st = '--newnames') then begin
    option_new_filenames := TRUE;
  end else if (st = '-m') or (st = '--rename') then begin
    option_rename := TRUE;
  end else begin
    writeln(stderr);
    writeln(stderr, 'Parameter unknown: ', st);
    DoParameter('?');
  end;
end;

var
  j : longint; st : AnsiString;

begin
  { Look for parameters: }
  for j := 1 to Paramcount do DoParameter(Paramstr(j));
  { Initialize some variables and the error-files: }
  Init;
  { Then we find and analyze the Header: }
  AnalyzeHeader;
  { As next, we parse the whole table, open each destination via "append"
    and construct the index-entry. }
  if not option_flatmode then
    writeln(f_debug);
  while not eof(input) do begin
    readln(input, st);
    inc(count);
    if option_flatmode then
      writeln(output, ConvertLine(st, ExtractByTitle(st, col_directory), option_use_ratings))
    else
      WorkAboutInputString(st);
    if not option_flatmode then
      writeln(f_debug);
  end;
  if not option_flatmode then begin
    writeln(stderr, 'Saving indizes.');
    writeln(f_debug, 'Saving indizes.');
    netplay.Save;
    tutorial.Save;
    for j := 1 to MaxIndizes do
      if index[j].name <> '' then
	index[j].Save;
    WriteIndexLua;
  end;
  { finally, output the number of CR's }
  if option_movefiles then
    writeln(f_notascii, 'Total number of deleted CRs: ', cr);
  closeandhalt;
end.







