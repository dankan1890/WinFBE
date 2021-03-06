'    WinFBE - Programmer's Code Editor for the FreeBASIC Compiler
'    Copyright (C) 2016-2019 Paul Squires, PlanetSquires Software
'
'    This program is free software: you can redistribute it and/or modify
'    it under the terms of the GNU General Public License as published by
'    the Free Software Foundation, either version 3 of the License, or
'    (at your option) any later version.
'
'    This program is distributed in the hope that it will be useful,
'    but WITHOUT any WARRANTY; without even the implied warranty of
'    MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE.  See the
'    GNU General Public License for more details.

#include once "modGenerateCode.bi"
#include once "clsControl.bi"



' ========================================================================================
' Set values of a WinFormsX Font class (used for code generation)
' ========================================================================================
private function SetFontClassFromPropValue( byref wszPropValue as wstring ) as CWSTR
   
   dim as CWSTR wszFont, wszStyles, wszCharSet
      
   if val(AfxStrParse(wszPropValue, 3, ",")) <> 400 then
      wszStyles = wszStyles & "FontStyles.Bold or "
   END IF
   if val(AfxStrParse(wszPropValue, 4, ",")) then
      wszStyles = wszStyles & "FontStyles.Italic or "
   END IF
   if val(AfxStrParse(wszPropValue, 5, ",")) then
      wszStyles = wszStyles & "FontStyles.Underline or "
   end if
   if val(AfxStrParse(wszPropValue, 6, ",")) then
      wszStyles = wszStyles & "FontStyles.StrikeOut or "
   end if
   wszStyles = rtrim(wszStyles, " or ")
   if len(wszStyles) = 0 then wszStyles = "FontStyles.Normal"

   select case val(AfxStrParse(wszPropValue, 7, ","))   ' charset
      case DEFAULT_CHARSET:     wszCharSet = "FontCharset.Default"
      case ANSI_CHARSET:        wszCharSet = "FontCharset.Ansi"        
      case ARABIC_CHARSET:      wszCharSet = "FontCharset.Arabic"      
      case BALTIC_CHARSET:      wszCharSet = "FontCharset.Baltic"      
      case CHINESEBIG5_CHARSET: wszCharSet = "FontCharset.ChineseBig5" 
      case EASTEUROPE_CHARSET:  wszCharSet = "FontCharset.EastEurope"  
      case GB2312_CHARSET:      wszCharSet = "FontCharset.GB2312"
      case GREEK_CHARSET:       wszCharSet = "FontCharset.Greek"      
      case HANGUL_CHARSET:      wszCharSet = "FontCharset.Hangul"      
      case HEBREW_CHARSET:      wszCharSet = "FontCharset.Hebrew"      
      case JOHAB_CHARSET:       wszCharSet = "FontCharset.Johab"       
      case MAC_CHARSET:         wszCharSet = "FontCharset.Mac"         
      case OEM_CHARSET:         wszCharSet = "FontCharset.OEM"         
      case RUSSIAN_CHARSET:     wszCharSet = "FontCharset.Russian"     
      case SHIFTJIS_CHARSET:    wszCharSet = "FontCharset.Shiftjis"    
      case SYMBOL_CHARSET:      wszCharSet = "FontCharset.Symbol"      
      case THAI_CHARSET:        wszCharSet = "FontCharset.Thai"        
      case TURKISH_CHARSET:     wszCharSet = "FontCharset.Turkish"     
      case VIETNAMESE_CHARSET:  wszCharSet = "FontCharset.Vietnamese"  
      case else: wszCharSet = "FontCharset.Default"
   end select
         
   wszFont = _
      chr(34) & AfxStrParse(wszPropValue, 1, ",") & chr(34) & "," & _
      AfxStrParse(wszPropValue, 2, ",") & "," & wszStyles & "," & wszCharSet

   function = wszFont

end function


' ========================================================================================
' Retrieve the Form name for this document.
' ========================================================================================
private function GetFormName( byval pDoc as clsDocument ptr ) as CWSTR
   if pDoc = 0 then exit function
   
   dim pCtrl as clsControl ptr
   dim as CWSTR wszFormName
   
   for i as long = pDoc->Controls.ItemFirst to pDoc->Controls.ItemLast
      pCtrl = pDoc->Controls.ItemAt(i)
      if (pCtrl <> 0) andalso (pCtrl->ControlType = CTRL_FORM) then
         wszFormName = GetControlProperty(pCtrl, "NAME")
         exit for
      end if
   next
   function = wszFormName   

end function


' ========================================================================================
' Generate the metadata that defines the form
' ========================================================================================
public function GenerateFormMetaData( byval pDoc as clsDocument ptr ) as long 
   if pDoc = 0 then exit function
   dim pCtrl as clsControl ptr
   dim wszControls as CWSTR
   
   ' Save Images(if applicable)
   dim as long numImageItems = Ubound(pDoc->AllImages) - lbound(pDoc->AllImages) + 1
   if numImageItems > 0 then
      wszControls = wszControls & "' WINFBE IMAGES_START" & vbcrlf 
      for ii as long = lbound(pDoc->AllImages) to ubound(pDoc->AllImages)
         wszControls = wszControls & _
              "'   IMAGE_START" & vbcrlf & _
              "'     IMAGENAME="     & pDoc->AllImages(ii).wszImageName & vbcrlf & _
              "'     FILENAME="      & ProcessToCurdrive(pDoc->AllImages(ii).wszFilename) & vbcrlf & _
              "'     RESOURCETYPE="  & pDoc->AllImages(ii).wszFormat & vbcrlf & _
              "'   IMAGE_END" & vbcrlf 
      next
      wszControls = wszControls & "' WINFBE IMAGES_END" & vbcrlf
   end if

   ' Save MainMenu (if applicable)
   dim as long numMenuItems = Ubound(pDoc->MenuItems) - lbound(pDoc->MenuItems) + 1
   if numMenuItems > 0 then
      wszControls = wszControls & _
         "' WINFBE MAINMENU_START" & vbcrlf & _
         "'   MAINMENU_DISPLAY=" & pDoc->GenerateMenu & vbcrlf
      for ii as long = lbound(pDoc->MenuItems) to ubound(pDoc->MenuItems)
         wszControls = wszControls & _
              "'   MENUITEM_START" & vbcrlf & _
              "'     NAME="     & pDoc->MenuItems(ii).wszName     & vbcrlf & _
              "'     CAPTION="  & pDoc->MenuItems(ii).wszCaption  & vbcrlf & _
              "'     INDENT="   & pDoc->MenuItems(ii).nIndent     & vbcrlf & _
              "'     ALT="      & pDoc->MenuItems(ii).chkAlt      & vbcrlf & _
              "'     SHIFT="    & pDoc->MenuItems(ii).chkShift    & vbcrlf & _
              "'     CTRL="     & pDoc->MenuItems(ii).chkCtrl     & vbcrlf & _
              "'     SHORTCUT=" & pDoc->MenuItems(ii).wszShortcut & vbcrlf & _
              "'     CHECKED="  & pDoc->MenuItems(ii).chkChecked  & vbcrlf & _
              "'     GRAYED="   & pDoc->MenuItems(ii).chkGrayed   & vbcrlf & _
              "'   MENUITEM_END" & vbcrlf 
      next
      wszControls = wszControls & "' WINFBE MAINMENU_END" & vbcrlf
   end if
   
   ' Save StatusBar Panels (if applicable)
   dim as long numPanelItems = Ubound(pDoc->PanelItems) - lbound(pDoc->PanelItems) + 1
   if numPanelItems > 0 then
      wszControls = wszControls & _
         "' WINFBE PANELS_START" & vbcrlf & _
         "'   STATUSBAR_DISPLAY=" & pDoc->GenerateStatusBar & vbcrlf
      for ii as long = lbound(pDoc->PanelItems) to ubound(pDoc->PanelItems)
         wszControls = wszControls & _
              "'   PANELITEM_START" & vbcrlf & _
              "'     PANELNAME="        & pDoc->PanelItems(ii).wszName        & vbcrlf & _
              "'     PANELTEXT="        & pDoc->PanelItems(ii).wszText        & vbcrlf & _
              "'     PANELTOOLTIP="     & pDoc->PanelItems(ii).wszTooltip     & vbcrlf & _
              "'     PANELALIGNMENT="   & pDoc->PanelItems(ii).wszAlignment   & vbcrlf & _
              "'     PANELBORDERSTYLE=" & pDoc->PanelItems(ii).wszBorderStyle & vbcrlf & _
              "'     PANELAUTOSIZE="    & pDoc->PanelItems(ii).wszAutoSize    & vbcrlf & _
              "'     PANELMINWIDTH="    & pDoc->PanelItems(ii).wszMinWidth    & vbcrlf & _
              "'   PANELITEM_END" & vbcrlf 
      next

      wszControls = wszControls & "'   EVENTS_START" & vbcrlf
      wszControls = wszControls & "'   EVENTS_END" & vbcrlf & _
                                  "' WINFBE PANELS_END" & vbcrlf
   end if
   
   
   ' Iterate all of the controls on the form
   for i as long = pDoc->Controls.ItemFirst to pDoc->Controls.ItemLast
      pCtrl = pDoc->Controls.ItemAt(i)
      if pCtrl then
         wszControls = wszControls & "' WINFBE CONTROL_START " & GetToolBoxName(pCtrl->ControlType) & vbcrlf & _
                                     "'   PROPERTIES_START" & vbcrlf 
         for ii as long = lbound(pCtrl->Properties) to ubound(pCtrl->Properties)
            wszControls = wszControls & _
                          "'     PROP_NAME="  & pCtrl->Properties(ii).wszPropName & vbcrlf & _
                          "'     PROP_VALUE=" & pCtrl->Properties(ii).wszPropValue & vbcrlf 
         NEXT
         wszControls = wszControls & "'   PROPERTIES_END" & vbcrlf & _
                                     "'   EVENTS_START" & vbcrlf 
         for ii as long = lbound(pCtrl->Events) to ubound(pCtrl->Events)
            ' Only need to output the names of the Events that have been ticked as being in use.
            if pCtrl->Events(ii).bIsSelected then
               wszControls = wszControls & _
                             "'     EVENT_NAME="   & pCtrl->Events(ii).wszEventName & vbcrlf 
            end if
         next
         wszControls = wszControls & "'   EVENTS_END" & vbcrlf & _
                                     "' WINFBE CONTROL_END" & vbcrlf
      END IF
   NEXT

   pDoc->wszFormMetaData = _
                 "' WINFBE FORM" & vbcrlf & _
                 "' WINFBE VERSION " & APPVERSION & vbcrlf & _
                 "' LOCKCONTROLS=" & iif(pDoc->bLockControls, "True", "False") & vbcrlf & _
                 "' SNAPLINES=" & iif(pDoc->bSnapLines, "True", "False") & vbcrlf & _
                 "' WINFBE FORM_START" & vbcrlf & _
                 wszControls & _
                 "' WINFBE FORM_END" & vbcrlf 
   
   function = 0
end function


' ========================================================================================
' Generate code for any existing Form MainMenu.
' ========================================================================================
private function GenerateMainMenuCode( byval pDoc as clsDocument ptr ) as CWSTR
   dim as clsMenuItem iMenu
   dim as CWSTR wszText, wszShortcut, wszFormName
   dim as CWSTR DQ = chr(34)
   dim as long nIndentParent, idxInsert 
   
   dim as string sp(5) 
   for i as long = 1 to ubound(sp)
      sp(i) = space(val(gConfig.TabSize) * i)
   NEXT

   wszText = _
   sp(1) & "dim ncm As NONCLIENTMETRICS" & vbcrlf & _
   sp(1) & "ncm.cbSize = SizeOf(ncm)" & vbcrlf & _
   sp(1) & "SystemParametersInfo(SPI_GETNONCLIENTMETRICS, SizeOf(ncm), @ncm, 0)" & vbcrlf & _
   sp(1) & "nClientOffset = AfxUnScaleY(ncm.iMenuHeight)  ' holds the height of the mainmenu" & vbcrlf & vbcrlf & _
   sp(1) & "this.MainMenu.Parent = @this" & vbcrlf 

   ' Every level of indent is a parent menu item
   ' First, create all of the menuitems. Once created, then we can add them 
   ' to their parent menuitem collections.
   for i as long = 0 to ubound(pDoc->MenuItems)
      iMenu = pDoc->MenuItems(i)
      if len(rtrim(iMenu.wszName)) = 0 then continue for
      
      wszShortcut = ""
      if iMenu.chkCtrl  then wszShortcut = wszShortcut & "Ctrl+"
      if iMenu.chkAlt   then wszShortcut = wszShortcut & "Alt+"
      if iMenu.chkShift then wszShortcut = wszShortcut & "Shift+"
      wszShortcut = wszShortcut & iMenu.wszShortcut
      
      wszText = wszText & sp(1) & _
         "dim " & iMenu.wszName & " as wfxMenuItem = wfxMenuItem(" & _
                  DQ & iMenu.wszCaption & DQ & ", " & _
                  DQ & iMenu.wszName & DQ & ", " & _
                  DQ & wszShortcut & DQ & ", " & _
                  iMenu.chkChecked & ", " & _
                  iMenu.chkGrayed & _
                  ")" & vbcrlf
   next
      
   dim as CWSTR wszNodes(ubound(pDoc->MenuItems))
   dim as CWSTR wszParentName
   dim as long NextFreeNode = 0
   
   ' Copy all of the root mainmenu items into the array first.
   for i as long = 0 to ubound(pDoc->MenuItems) 
      iMenu = pDoc->MenuItems(i)
      if len(rtrim(iMenu.wszName)) = 0 then continue for
      if iMenu.nIndent = 0 then
         wszNodes(NextFreeNode) = sp(1) & "this.MainMenu.MenuItems.Add(" & iMenu.wszName & ")" 
         NextFreeNode = NextFreeNode + 1
      end if
   next
   
   ' Add the child menuitems to their parents 
   for i as long = 0 to ubound(pDoc->MenuItems) 
      iMenu = pDoc->MenuItems(i)
      if iMenu.nIndent = 0 then continue for
      if len(rtrim(iMenu.wszName)) = 0 then continue for

      ' Determine where to insert the string into the array by first determining the
      ' parent node of this menuitem and then insert the menuitem immediately before it.
      ' If not found then insert at the end of the array (these would be the mainmenu
      ' root items).
      
      ' This is a child popup menuitem so we need to find the parent
      ' menutitem and add this item to that collection
      nIndentParent = iMenu.nIndent - 1 
      for ii as long = i to 0 step -1
         if pDoc->MenuItems(ii).nIndent = nIndentParent then
            wszParentName = pDoc->MenuItems(ii).wszName
            exit for
         end if       
      next    
      ' Search the array for the line that adds the parent node to determine the
      ' insertion point.
      for ii as long = 0 to ubound(wszNodes)
         if instr(wszNodes(ii), "MenuItems.Add(" & wszParentName & ")" ) then
            ' Do the actual inserting into the array
            idxInsert = ii
            for yy as long = ubound(wszNodes) to idxInsert + 1 step - 1
                wszNodes(yy) = wszNodes(yy-1)
            next
            wszNodes(idxInsert) = sp(1) & wszParentName & ".MenuItems.Add(" & iMenu.wszName & ")" 
            exit for
         end if   
      next

   next


   ' Join all of the node strings together
   for i as long = 0 to ubound(wszNodes) 
      wszText = wszText & wszNodes(i) & vbcrlf
   next   


   wszFormName = GetFormName(pDoc)
   wszText = wszText & _
      sp(1) & "this.MainMenu.OnPopup = @" & wszFormName & "_MainMenu_Popup" & vbcrlf & _
      sp(1) & "this.MainMenu.OnClick = @" & wszFormName & "_MainMenu_Click" & vbcrlf & _
      sp(1) & "this.Controls.Add(ControlType.MainMenu, @this.MainMenu)" & vbcrlf & vbcrlf

   return wszText
end function


' ========================================================================================
' Generate (or regenerate) visual designer code.
' ========================================================================================
public function GenerateFormCode( byval pDoc as clsDocument ptr ) as long
   if pDoc = 0 then exit function
   if pDoc->IsDesigner = false then exit function
   if pDoc->bRegenerateCode = false then exit function
   
   dim pCtrl as clsControl ptr
   dim as CWSTR wszText, wszFormName, wszCtrlName, wszPropName, wszPropValue, wszPropDefault
   dim as CWSTR wszFunction, wszAllEvents, wszPrototype, wszAllDeclares
   dim as CWSTR wszCombinedName, wszCodeGen, wszMenuSelect
   dim as CWSTR wszStartTag, wszEndTag
   dim as CWSTR DQ = chr(34)
   dim as long nStartTag, nEndTag, nPropType
   dim pData as DB2_DATA ptr
   
   dim as string sp(5) 
   for i as long = 1 to ubound(sp)
      sp(i) = space(val(gConfig.TabSize) * i)
   NEXT
   
   pDoc->wszFormCodegen = ""

   wszFormName = GetFormName(pDoc)
   wszStartTag = "' WINFBE_CODEGEN_START"
   wszEndTag   = "' WINFBE_CODEGEN_END"

   wszText = "#include once " & DQ & "WinFormsX\WinFormsX.bi" & DQ & vbcrlf 

   ''
   ''  Generate the Declares for all of the Events for any MainMenu for the Form 
   ''
   if pDoc->MainMenuExists then
      dim as CWSTR wszMenuEvents(1)
      wszMenuEvents(0) = "Click"
      wszMenuEvents(1) = "Popup"
      
      for i as long = lbound(wszMenuEvents) to ubound(wszMenuEvents)
         ' Determine the name of the event function
         wszFunction = wszFormName & "_MainMenu_" & wszMenuEvents(i)
         
         ' Define the function prototype/declaration
         wszPrototype = "Function " & wszFunction & "( ByRef sender As wfxMenuItem" & _
                           ", ByRef e As EventArgs ) As LRESULT" & vbcrlf

         ' Add the event/function to the list of declares
         wszAllDeclares = wszAllDeclares & "Declare " & wszPrototype
         
         ' If the Event/function does not already exist in code then create it now.
         pDoc->bNeedsParsing = true
         ParseDocument( pDoc )
         
         ' Only check if we are not loading the project because the event handling
         ' could already exist but just be in another source code file that has
         ' not yet been loaded.
         if (gApp.IsProjectLoading = false) andalso (gApp.IsFileLoading = false) then
            pData = gdb2.dbFindFunction(wszFunction)
            if pData = 0 THEN
               wszMenuSelect = sp(1) & "Select Case UCase(sender.Name)" & vbcrlf 
               for ii as long = 0 to ubound(pDoc->MenuItems) 
                  if len(rtrim(pDoc->MenuItems(ii).wszName)) = 0 then continue for
                  ' Popups only needed for menuitems with IsParent. If the indent level
                  ' of the next menuitem is greater, then this is a Parent menuitem.
                  if i = 1 then
                     if ii = ubound(pDoc->MenuItems) then continue for
                     if pDoc->MenuItems(ii+1).nIndent <= pDoc->MenuItems(ii).nIndent then continue for
                  end if   
                  wszMenuSelect = wszMenuSelect & _
                        sp(2) & "Case " & chr(34) & ucase(pDoc->MenuItems(ii).wszName) & chr(34) & vbcrlf
               next
               ' Must add a CASE ELSE because if no child menu items then SELECT CASE will be 
               ' empty causing a compile time error.   
               wszMenuSelect = wszMenuSelect & _
                     sp(2) & "Case Else" & vbcrlf & _
                     sp(1) & "End Select" & vbcrlf 
               
               wszAllEvents = wszAllEvents & "''" & vbcrlf & "''" & vbcrlf & _
                              wszPrototype & _
                              wszMenuSelect & _
                              sp(1) & "Function = 0" & vbcrlf & _
                              "End Function" & vbcrlf & vbcrlf
            end if
         end if
      next
   end if

   
   ''
   ''  Generate the Declares for all of the Events for the Form & Controls
   ''
   for i as long = pDoc->Controls.ItemFirst to pDoc->Controls.ItemLast
      pCtrl = pDoc->Controls.ItemAt(i)
      if pCtrl then
         wszCtrlName = GetControlProperty(pCtrl, "NAME")
         for ii as long = lbound(pCtrl->Events) to ubound(pCtrl->Events)
            if pCtrl->Events(ii).bIsSelected then
               ' Determine the name of the event function
               wszFunction = wszFormName
               if pCtrl->ControlType <> CTRL_FORM then wszFunction = wszFunction & "_" & wszCtrlName
               wszFunction = wszFunction & "_" & pCtrl->Events(ii).wszEventName 
               
               ' Define the function prototype/declaration
               wszPrototype = "Function " & wszFunction & "( ByRef sender As " & _
               GetWinformsXClassName(pCtrl->ControlType) & ", ByRef e As EventArgs ) As LRESULT" & vbcrlf

               ' Add the event/function to the list of declares
               wszAllDeclares = wszAllDeclares & "Declare " & wszPrototype
               
               ' If the Event/function does not already exist in code then create it now.
               pDoc->bNeedsParsing = true
               ParseDocument( pDoc )
               
               ' Only check if we are not loading the project because the event handling
               ' could already exist but just be in another source code file that has
               ' not yet been loaded.
               if (gApp.IsProjectLoading = false) andalso (gApp.IsFileLoading = false) then
                  pData = gdb2.dbFindFunction(wszFunction)
                  if pData = 0 THEN
                     wszAllEvents = wszAllEvents & "''" & vbcrlf & "''" & vbcrlf & _
                                    wszPrototype & _
                                    sp(1) & "Function = 0" & vbcrlf & _
                                    "End Function" & vbcrlf & vbcrlf
                  end if
               end if
            end if   
         next
      END IF
   next   

   
   ''
   ''  Generate the TYPE definition for the Form
   ''
   wszText = wszText & wszAllDeclares & iif(len(wszAllDeclares), vbcrlf, "") & _
   "type " & wszFormName & "Type extends wfxForm" & vbcrlf & _
   sp(1) & "private:"              & vbcrlf & _
   sp(2) & "temp as byte"          & vbcrlf & _
   sp(1) & "public:"               & vbcrlf & _
   sp(2) & "declare constructor"   & vbcrlf & _
   sp(2) & "' Controls"            & vbcrlf 
      for i as long = pDoc->Controls.ItemFirst to pDoc->Controls.ItemLast
         pCtrl = pDoc->Controls.ItemAt(i)
         if pCtrl then
            if pCtrl->ControlType = CTRL_FORM then continue for
            wszCtrlName = GetControlProperty(pCtrl, "NAME")
            wszText = wszText & sp(2) & wszCtrlName & " As " & GetWinformsXClassName(pCtrl->ControlType) & vbcrlf
         END IF
      next   
   wszText = wszText & _
   "end type" & vbcrlf & vbcrlf 

   ''
   ''  Generate the Constructor that defines the properties for the form and controls
   ''
   wszText = wszText & _
   "constructor " & wszFormName & "Type" & vbcrlf & _
   sp(1) & "dim as long nClientOffset" & vbcrlf & vbcrlf 

   ' If a menu exists for the Form then add the code generation
   if pDoc->MainMenuExists then
      wszText = wszText & GenerateMainMenuCode(pDoc)
   end if
   
   for i as long = pDoc->Controls.ItemFirst to pDoc->Controls.ItemLast
      pCtrl = pDoc->Controls.ItemAt(i)

      if pCtrl then
         wszCtrlName = GetControlProperty(pCtrl, "NAME")

         wszCombinedName = "this" 
         if pCtrl->ControlType <> CTRL_FORM then
            wszCombinedName = wszCombinedName & "." & **wszCtrlName
         end if
            
         if pCtrl->ControlType <> CTRL_FORM then 
            wszText = wszText & sp(1) & wszCombinedName & ".Parent = @this" & vbcrlf
         END IF

         dim as long nLeft, nTop, nWidth, nHeight
         for ii as long = lbound(pCtrl->Properties) to ubound(pCtrl->Properties)
            wszPropName    = pCtrl->Properties(ii).wszPropName
            wszPropValue   = pCtrl->Properties(ii).wszPropValue
            wszPropDefault = pCtrl->Properties(ii).wszPropDefault
            nPropType      = pCtrl->Properties(ii).PropType
            
            select case ucase(wszPropName)
               CASE "LEFT"
                  nLeft = wszPropValue.ValLong
                  continue for
               case "TOP"
                  nTop = wszPropValue.ValLong
                  continue for
               case "WIDTH"
                  nWidth = wszPropValue.ValLong
                  continue for
               case "HEIGHT"
                  nHeight = wszPropValue.ValLong
                  continue for
               case "NAME"  ' don't output NAME property b/c it can't be changed programmically
                  continue for
               case "TABINDEX"
                  continue for   
            END SELECT

            ' Only output properties that are different than the default WinFormsX values
            if ucase(wszPropValue) <> ucase(wszPropDefault) then
               select case ucase(wszPropName)
                  case "TEXT", "TAG", "PASSWORDCHAR", "GROUPNAME", _
                       "MASKSTRING", "INPUTSTRING", "DEFAULTCHARACTER", "VALIDCHARACTERS", _
                       "ICON", "IMAGE"            
                     ' Ensure that any embedded double quotes are escaped
                     wszPropValue = AfxStrReplace(wszPropValue, DQ, DQ & " & chr(34) & " & DQ)
                     wszPropValue = DQ & wszPropValue & DQ
                  case "ACCEPTBUTTON", "CANCELBUTTON"
                     wszPropValue = "@this." & wszPropValue 
               end select
               select case nPropType
                  CASE PropertyType.ColorPicker
                     if left(wszPropValue, 7) = "SYSTEM|" then
                        wszPropValue = "Colors.System" & mid(**wszPropValue, 8)
                     elseif left(wszPropValue, 7) = "COLORS|" then
                        wszPropValue = "Colors." & mid(**wszPropValue, 8)
                     elseif left(wszPropValue, 7) = "CUSTOM|" then
                        wszPropValue = mid(**wszPropValue, 8) & "  ' custom color"
                     end if   
                  case PropertyType.FontPicker
                     wszPropValue = "New wfxFont(" & SetFontClassFromPropValue(wszPropValue) & ")"
               END SELECT
               wszText = wszText & sp(1) & wszCombinedName & "." & wszPropName & " = " & wszPropValue & vbcrlf
            end if
         NEXT
         if pCtrl->ControlType = CTRL_FORM then 
            wszText = wszText & sp(1) & wszCombinedName & ".SetBounds(" & nLeft & "," & nTop & "," & nWidth & "," & nHeight & ")" & vbcrlf
         else
            wszText = wszText & sp(1) & wszCombinedName & ".SetBounds(" & nLeft & "," & nTop & "-nClientOffset," & nWidth & "," & nHeight & ")" & vbcrlf
         end if


         for ii as long = lbound(pCtrl->Events) to ubound(pCtrl->Events)
            if pCtrl->Events(ii).bIsSelected then
               ' Determine the name of the event function
               wszFunction = wszFormName
               if pCtrl->ControlType <> CTRL_FORM then wszFunction = wszFunction & "_" & wszCtrlName
               wszFunction = wszFunction & "_" & pCtrl->Events(ii).wszEventName 
               wszText = wszText & sp(1) & wszCombinedName & ".On" & pCtrl->Events(ii).wszEventName & " = @" & wszFunction & vbcrlf
            end if
         next   

      end if
   next   
   
   
   ''
   ''  Add the controls to the form collection (we first need to sort based on TabIndex).
   ''
   dim pProp as clsProperty ptr
   dim TabOrder(pDoc->Controls.Count) as TABORDER_TYPE
   dim nTabIndex as Long
   
   ' Load the temporary array
   for i as long = pDoc->Controls.ItemFirst to pDoc->Controls.ItemLast
      pCtrl = pDoc->Controls.ItemAt(i)
      if pCtrl then
         TabOrder(i).pCtrl = pCtrl
         pProp = GetControlPropertyPtr(pCtrl, "TABINDEX")
         if pProp then
            nTabIndex = val(pProp->wszPropValue)
            pProp = GetControlPropertyPtr(pCtrl, "TABSTOP")
            if pProp = 0 then nTabIndex = 999999
         else
            nTabIndex = 999999   
         end if
         TabOrder(i).TabIndex = nTabIndex
      end if   
   next


   ' Sort the array using a simple bubble sort
   Dim As Long lb = LBound(TabOrder)
   Dim As Long ub = UBound(TabOrder)
   Dim As boolean bHasChanged
   Do
      bHasChanged = false
      For i as long = lb To ub - 1
         If TabOrder(i).TabIndex > TabOrder(i+1).TabIndex Then
            Swap TabOrder(i), TabOrder(i+1)
            bHasChanged = true
         End If
      Next
   Loop Until bHasChanged = false

   ' Finally, output the controls 
   dim as CWSTR wszGroupName, wszTemp
   for i as long = lb to ub 
      pCtrl = TabOrder(i).pCtrl
      if pCtrl then
         if pCtrl->ControlType <> CTRL_FORM then 
            ' If this is an optionbutton then add a group for each change in the 
            ' groupname value.
            wszCtrlName = GetControlProperty(pCtrl, "NAME")
            if pCtrl->ControlType = CTRL_OPTION then
               wszTemp = ucase(GetControlProperty(pCtrl, "GROUPNAME"))
               if wszTemp <> wszGroupName then
                  wszText = wszText & _
                  sp(1) & "this." & wszCtrlName & ".StartGroup = True" & vbcrlf
                  wszGroupName = wszTemp 
               end if   
            end if
            wszText = wszText & _
            sp(1) & "this.Controls.Add(ControlType." & GetToolBoxName(pCtrl->ControlType) & ", @this." & wszCtrlName & ")" & vbcrlf
         end if
      end if
   next
   
   ''
   ''  Add the form to the application
   ''
   wszText = wszText & _
   sp(1) & "Application.Forms.Add(ControlType.Form, @this)" & vbcrlf & _
   "end constructor" & vbcrlf & vbcrlf

   wszText = wszText & _ 
   "dim shared " & wszFormName & " as " & wszFormName & "Type" & vbcrlf
   
   wszCodeGen = wszStartTag & vbcrlf & wszText & wszEndTag & vbcrlf 
                     
   dim as any ptr pSci = pDoc->GetActiveScintillaPtr()
   if SciMsg( pSci, SCI_GETLENGTH, 0 , 0) = 0 then
      ' If this is the first time generating code then add the boilerplate code
      ' to start the application and show the form.
      dim as CWSTR wszHeader
      if gApp.IsProjectActive = false then
         wszHeader = wszHeader & _
         "' You should always include a resource file that references a valid manifest.xml"      & vbcrlf & _
         "' file otherwise your application will not properly display Windows themed controls."  & vbcrlf & _
         "' Sample resource.rc and manifest.xml files can be found in the WinFBE \Settings folder."   & vbcrlf & _
         "' The following WinFBE directive includes the resource in your application. Simply" & vbcrlf & _
         "' uncomment the line." & vbcrlf & _
         "' If you are using WinFBE's project management features then delete the following line" & vbcrlf & _
         "' because a resource file will be generated automatically." & vbcrlf & _
         "'     '#RESOURCE " & DQ & "resource.rc" & DQ & vbcrlf & vbcrlf 
      END IF
      wszHeader = wszHeader & vbcrlf & _
      "''" & vbcrlf & _
      "''  Remove the following Application.Run code if it used elsewhere in your application." & vbcrlf & _
      "Application.Run(" & wszFormName & ")" & vbcrlf & vbcrlf
      if pDoc->IsNewFlag then
         pDoc->AppendText(wszHeader)
      end if   
   end if

   ' If any new Events were discovered then append them to the code editor
   if len(wszAllEvents) then
      pDoc->AppendText( wszAllEvents )
      pDoc->bNeedsParsing = true
      ParseDocument(pDoc)
   END IF
  
   ' Save the generated codegen code because we will output it to a disk file
   pDoc->wszFormCodeGen = wszCodeGen
   pDoc->bRegenerateCode = false
   
   function = 0
end function

