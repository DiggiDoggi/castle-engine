{%MainUnit castleglutils.pas}
{
  Copyright 2023-2023 Michalis Kamburelis.

  This file is part of "Castle Game Engine".

  "Castle Game Engine" is free software; see the file COPYING.txt,
  included in this distribution, for details about the copyright.

  "Castle Game Engine" is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  ----------------------------------------------------------------------------
}

{$ifdef read_interface}

type
  { Vertex array object.
    Wrapper over VAO, as defined by OpenGL >= 3.

    Note that you can use this from multiple OpenGL(ES) contexts,
    it will automatically create and destroy VAOs specific to the current context.
    (As VAOs are not shared between OpenGL(ES) contexts,
    see https://stackoverflow.com/questions/55885139/what-is-shareable-between-opengl-contexts-and-how-to-enable-sharing .)

    This does nothing if current OpenGL(ES) version doesn't support VAO.

    Change the current VAO by @link(TRenderContext.CurrentVao RenderContext.CurrentVao).
    It is done done automatically by @link(TGLSLAttribute.EnableArray),
    remember that VBOs (including GL_ELEMENT_ARRAY_BUFFER) must be bound within this VAO. }
  TVertexArrayObject = class(TComponent)
  strict private
    type
      TContextVaoDictionary = {$ifdef FPC}specialize{$endif} TDictionary<TComponent, TGLVertexArrayObject>;
    var
      ContextsVao: TContextVaoDictionary;
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    { OpenGL VAO identifier, for given context (must be @link(TRenderContext)). }
    function InternalHandle(const Context: TComponent): TGLVertexArrayObject;
  end;

{$endif read_interface}

{$ifdef read_implementation}

constructor TVertexArrayObject.Create(AOwner: TComponent);
begin
  inherited;
  ContextsVao := TContextVaoDictionary.Create;
end;

destructor TVertexArrayObject.Destroy;
var
  Pair: {$ifdef FPC}TContextVaoDictionary.TDictionaryPair{$else}
    TPair<TComponent, TGLVertexArrayObject>{$endif};
begin
  if ContextsVao <> nil then
  begin
    for Pair in ContextsVao do
    begin
      Pair.Key.RemoveFreeNotification(Self);
      if GLFeatures.VertexArrayObject then
      begin
        Assert(Pair.Value <> GLObjectNone);
        glDeleteVertexArray(Pair.Value);
      end;
    end;
    FreeAndNil(ContextsVao);
  end;
  inherited;
end;

procedure TVertexArrayObject.Notification(AComponent: TComponent; Operation: TOperation);
var
  Vao: TGLVertexArrayObject;
begin
  inherited;

  { When TRenderContext is freed, we must free VAOs associated with it. }
  if Operation = opRemove then
    if ContextsVao.TryGetValue(AComponent, Vao) then
    begin
      FreeVertexArray(Vao);
      ContextsVao.Remove(AComponent);
    end;
end;

function TVertexArrayObject.InternalHandle(const Context: TComponent): TGLVertexArrayObject;
begin
  Assert(Context is TRenderContext);

  if not ContextsVao.TryGetValue(Context, Result) then
  begin
    if GLFeatures.VertexArrayObject then
      Result := glCreateVertexArray()
    else
      Result := GLObjectNone;
    Context.FreeNotification(Self);
    ContextsVao.Add(Context, Result);
  end;
end;

{$endif read_implementation}
