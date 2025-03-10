{ -*- buffer-read-only: t -*-

  Copyright 2015-2025 Michalis Kamburelis.

  This file is part of "Castle Game Engine".

  "Castle Game Engine" is free software; see the file COPYING.txt,
  included in this distribution, for details about the copyright.

  "Castle Game Engine" is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  ----------------------------------------------------------------------------
}

{ Automatically generated node properties.

  Do not edit this file manually!
  To add new properties:
  - add them to the text files in tools/internal/x3d-nodes-to-pascal/nodes-specification/ ,
  - and regenerate include files by running x3d-nodes-to-pascal }

{$ifdef read_interface}

  strict private
    function GetMode: TColorMode;
    procedure SetMode(const Value: TColorMode);
  public
    { Create node fields and events. }
    procedure CreateNode; override;
    class function ClassX3DType: String; override;

    strict private FFdMode: TSFStringEnum;
    { Internal wrapper for property @code(Mode). This wrapper API may change, we advise to access simpler @code(Mode) instead, if it is defined (TODO: for now, some field types do not have a simpler counterpart). }
    public property FdMode: TSFStringEnum read FFdMode;
    {  }
    property Mode: TColorMode read GetMode write SetMode;

{$endif read_interface}

{$ifdef read_implementation}

{ TAbstractColorNode ----------------------------------------------- }

function TAbstractColorNode.GetMode: TColorMode;
begin
  Result := TColorMode(FdMode.EnumValue);
end;

procedure TAbstractColorNode.SetMode(const Value: TColorMode);
begin
  FdMode.SendEnumValue(Ord(Value));
end;

class function TAbstractColorNode.ClassX3DType: String;
begin
  Result := 'X3DColorNode';
end;

procedure TAbstractColorNode.CreateNode;
begin
  inherited;

  FFdMode := TSFStringEnum.Create(Self, False, 'mode', @ColorModeNames, Ord(High(ColorModeNames)) + 1, Ord(cmReplace));
   FdMode.ChangeAlways := chVisibleNonGeometry;
  AddField(FFdMode);

  DefaultContainerField := 'color';
end;

{$endif read_implementation}
