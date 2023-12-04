program main;
{$mode objfpc}

uses 
StrUtils, Sysutils, crt;

const
C_FNAME = 'cards.txt';

type
  mArray = array[0..99] of Integer;
  
var
myFile      : TextFile;
myArr       : mArray;
arrayLength : Integer;
lineLength  : Integer;
pos_i       : Integer;
pos_j       : Integer;
i           : Integer;
k           : Integer;
points      : Integer;
res         : Integer;
left        : string;
right       : string;
line        : string;
num         : string;
arrayLeft   : TStringArray;
arrayRight  : TStringArray;

procedure ResetArray(var A: mArray);
var
  i: Integer;
begin
  for i := Low(A) to High(A) do
    A[i] := 0;
end;

procedure RemoveNonNumbers(var A: TStringArray);
var
  i : Integer;
  j : Integer; 
  k : Integer; 
  
begin
  j := 0; 
  for i := Low(A) to High(A) do
  begin
    if TryStrToInt(A[i], k) then
    begin
      A[j] := A[i];
      Inc(j);
    end;
  end;
  SetLength(A, j);
end;

begin
  // assign file
  AssignFile(myFile, C_FNAME);

  // open file
  Reset(myFile);

  // initialize array
  ResetArray(myArr);

  // initialize result
  res := 0;
  
  // read file
  while not Eof(myFile) do
  begin    
    ReadLn(myFile, line);
    lineLength := Length(line);
    
    pos_i := Pos(':', line) + 1;
    pos_j := Pos('|', line) + 1;
    left := Copy(line, pos_i, (pos_j - pos_i) - 1);
    right := Copy(line, pos_j, (lineLength - pos_j) + 1);
    
    left := Trim(left);
    right := Trim(right);
    arrayLeft := SplitString(left, ' ');
    arrayRight := SplitString(right, ' ');

    RemoveNonNumbers(arrayLeft);
    RemoveNonNumbers(arrayRight);
    
    for i := Low(arrayLeft) to High(arrayLeft) do
      begin
        num := arrayLeft[i];
        try
          k := StrToInt(num);
          myArr[k] := 1;
        except
          on E: EConvertError do
            Writeln('0:Error converting string to integer: ', E.Message);
        end;
      end;

    points := 0;
    for i := Low(arrayRight) to High(arrayRight) do
      begin
        num := arrayRight[i];
        try
          k := StrToInt(num);
          if (myArr[k] = 1) then
            begin
              if (points = 0) then points := 1
              else points := points * 2;
            end
        except
          on E: EConvertError do
            Writeln('1:Error converting string to integer: ', E.Message);
        end;
      end;
    res := res + points;
    
    // reset the array for next line
    ResetArray(myArr);
  end;

  writeln(res);

  // close file
  CloseFile(myFile);
end.