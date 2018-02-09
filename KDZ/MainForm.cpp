#include "MainForm.h"
#include "Scene.h"
#include "Renderer.h"

#include <Windows.h>

using namespace KDZ;

[STAThread]
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MainForm);
	return 0;
}

System::Void MainForm::setScene() {
	checkButtons();
	updateObjectParams();
	updateCameraParams();
	updateOtherParams();
	renderScene();
}

System::Void MainForm::renderScene() {
	Graphics ^gr = Graphics::FromImage(bm);
	try {
		renderer->setGraphics(gr);
		mainScene->renderScene(renderer);
		updateStatusBar();
	}
	finally {
		delete gr;
	}
	pictureBox->Refresh();
}

System::Void MainForm::checkButtons() {
	if (mainScene->isEmpty()) {
		prevObjButton->Enabled = false;
		nextObjButton->Enabled = false;
		resetObjButton->Enabled = false;
		deleteObjButton->Enabled = false;
		objPosXBar->Enabled = false;
		objPosYBar->Enabled = false;
		objPosZBar->Enabled = false;
		objRotXBar->Enabled = false;
		objRotYBar->Enabled = false;
		objRotZBar->Enabled = false;
		objScaleXBar->Enabled = false;
		objScaleYBar->Enabled = false;
		objScaleZBar->Enabled = false;
		objReflectionXYCheckbox->Enabled = false;
		objReflectionXZCheckbox->Enabled = false;
		objReflectionYZCheckbox->Enabled = false;
	}
	else {
		prevObjButton->Enabled = mainScene->objectCount() > 1 && !mainScene->isSelectedFirst();
		nextObjButton->Enabled = mainScene->objectCount() > 1 && !mainScene->isSelectedLast();
		resetObjButton->Enabled = true;
		deleteObjButton->Enabled = true;
		objPosXBar->Enabled = true;
		objPosYBar->Enabled = true;
		objPosZBar->Enabled = true;
		objRotXBar->Enabled = true;
		objRotYBar->Enabled = true;
		objRotZBar->Enabled = true;
		objScaleXBar->Enabled = true;
		objScaleYBar->Enabled = true;
		objScaleZBar->Enabled = true;
		objReflectionXYCheckbox->Enabled = true;
		objReflectionXZCheckbox->Enabled = true;
		objReflectionYZCheckbox->Enabled = true;
	}
}

System::Void MainForm::updateStatusBar() {
	String^ info = "Objects : " + mainScene->objectCount() + " | ";
	// get the current object's and camera's info
	if (!mainScene->isEmpty()) {
		GL::Vector3 pos = mainScene->getObjectPosition(true);
		info += "Pos : (" + pos.x + ", " + pos.y + ", " + pos.z + ") | ";
		GL::Vector3 rot = mainScene->getObjectRotation(true);
		info += "Rot : (" + rot.x + ", " + rot.y + ", " + rot.z + ") | ";
		GL::Vector3 sca = mainScene->getObjectScale(true);
		info += "Scale : (" + sca.x + ", " + sca.y + ", " + sca.z + ") | ";
	}
	GL::Vector3 camPos = mainScene->getCameraPosition(true);
	info += "Camera | Pos : (" + camPos.x + ", " + camPos.y + ", " + camPos.z + ") | ";
	GL::Vector3 camRot = mainScene->getCameraRotation(true);
	info += "Pitch : " + camRot.x + " | Yaw : " + camRot.y + " |";
	objCountLabel->Text = info;
}

System::Void MainForm::openToolStripMenuItem_Click(System::Object ^ sender, System::EventArgs ^ e) {
	if (openFileDialog->ShowDialog() == ::DialogResult::OK) {
		IO::StreamReader ^sr = gcnew IO::StreamReader(openFileDialog->FileName);
		try {;
			if (mainScene->fromFile(sr->ReadToEnd())) {
				// object loaded successfully
				checkButtons();
				updateObjectParams();
				renderScene();
			}
			else {
				// something went wrong
				MessageBox::Show(L"Invalid file format.", L"Error");
			}
		}
		finally {
			sr->Close();
		}
	}
}

System::Void MainForm::exitToolStripMenuItem_Click(System::Object ^ sender, System::EventArgs ^ e) {
	Application::Exit();
}

System::Void MainForm::setObjectsParams(int objPosX, int objPosY, int objPosZ, int objScaleX, int objScaleY, int objScaleZ, int objRotX, int objRotY, int objRotZ, bool objReflXY, bool objReflXZ, bool objReflYZ) {
	isSettingParams = true;
	// set object's trackbars
	objPosXBar->Value = objPosX;
	objPosYBar->Value = objPosY;
	objPosZBar->Value = objPosZ;
	objScaleXBar->Value = objScaleX;
	objScaleYBar->Value = objScaleY;
	objScaleZBar->Value = objScaleZ;
	objRotXBar->Value = objRotX;
	objRotYBar->Value = objRotY;
	objRotZBar->Value = objRotZ;
	objReflectionXYCheckbox->Checked = objReflXY;
	objReflectionXZCheckbox->Checked = objReflXZ;
	objReflectionYZCheckbox->Checked = objReflYZ;
	isSettingParams = false;
}

System::Void MainForm::setCameraParams(int camPosX, int camPosY, int camPosZ, int camPitch, int camYaw, bool perspective) {
	// set camera's trackbars
	isSettingParams = true;
	camPosXBar->Value = camPosX;
	camPosYBar->Value = camPosY;
	camPosZBar->Value = camPosZ;
	camRotPitchBar->Value = camPitch;
	camRotYawBar->Value = camYaw;
	perspectiveButton->Checked = perspective;
	orthoButton->Checked = !perspective;
	isSettingParams = false;
}

System::Void MainForm::setOtherParams(Color bgColor, Color wfColor, Color selectedColor, bool wfMode, bool solidMode, bool faceCull) {
	// set other parameters
	isSettingParams = true;
	wfColorButton->BackColor = wfColor;
	bgColorButton->BackColor = bgColor;
	selectedColorButton->BackColor = selectedColor;
	if (wfMode && solidMode) {
		bothRadioButton->Checked = true;
	}
	else {
		wfRadioButton->Checked = wfMode;
		solidRadioButton->Checked = solidMode;
	}
	cullOnRadioButton->Checked = faceCull;
	cullOffRadioButton->Checked = !faceCull;
	isSettingParams = false;
}

System::Void MainForm::updateObjectParams() {
	if (!mainScene->isEmpty()) {
		GL::Vector3 pos = mainScene->getObjectPosition(false);
		GL::Vector3 rot = mainScene->getObjectRotation(false);
		GL::Vector3 sc = mainScene->getObjectScale(false);
		GL::Vector3 refl = mainScene->getObjectReflection();
		setObjectsParams(pos.x, pos.y, pos.z, sc.x, sc.y, sc.z, rot.x, rot.y, rot.z, refl.z, refl.y, refl.x);
	}
}

System::Void MainForm::updateCameraParams() {
	GL::Vector3 pos = mainScene->getCameraPosition(false);
	GL::Vector3 rot = mainScene->getCameraRotation(false);
	bool perspective = mainScene->isPerspective();
	setCameraParams(pos.x, pos.y, pos.z, rot.x, rot.y, perspective);
}

System::Void MainForm::updateOtherParams() {
	Color bg = renderer->getBGColor();
	Color wfCol = renderer->getWFColor();
	Color sel = renderer->getSelectedColor();
	bool wf = mainScene->isWireframeMode();
	bool solid = mainScene->isSolidMode();
	bool cull = mainScene->isCulling();
	setOtherParams(bg, wfCol, sel, wf, solid, cull);
}

// Sets scene after the MainForm is shown.
System::Void MainForm::MainForm_Shown(System::Object^  sender, System::EventArgs^  e) {
	setScene();
}

System::Void MainForm::MainForm_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
	bm = gcnew Bitmap(pictureBox->Width, pictureBox->Height);
	renderer->setGraphics(Graphics::FromImage(bm));
	renderer->setViewport(pictureBox->Width, pictureBox->Height);
	pictureBox->Image = bm;
	renderScene();
}

// Object position scroll bars:

System::Void MainForm::changeObjectPosition() {
	mainScene->setObjectPosition(objPosXBar->Value, objPosYBar->Value, objPosZBar->Value);
	renderScene();
}

System::Void MainForm::objPosXBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectPosition();
}

System::Void MainForm::objPosYBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectPosition();
}

System::Void MainForm::objPosZBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectPosition();
}

// Object scale scroll bars:

System::Void MainForm::changeObjectScale() {
	mainScene->setObjectScale(objScaleXBar->Value, objScaleYBar->Value, objScaleZBar->Value);
	renderScene();
}

System::Void MainForm::objScaleXBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectScale();
}

System::Void MainForm::objScaleYBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectScale();
}

System::Void MainForm::objScaleZBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectScale();
}

// Object rotation scroll bars:

System::Void MainForm::changeObjectRotation() {
	mainScene->setObjectRotation(objRotXBar->Value, objRotYBar->Value, objRotZBar->Value);
	renderScene();
}

System::Void MainForm::objRotXBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectRotation();
}

System::Void MainForm::objRotYBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectRotation();
}

System::Void MainForm::objRotZBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeObjectRotation();
}

// Camera position scroll bars:

System::Void MainForm::changeCameraPosition() {
	mainScene->setCameraPosition(camPosXBar->Value, camPosYBar->Value, camPosZBar->Value);
	renderScene();
}

System::Void MainForm::camPosXBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeCameraPosition();
}

System::Void MainForm::camPosYBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeCameraPosition();
}

System::Void MainForm::camPosZBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeCameraPosition();
}

// Camera rotation scroll bars:

System::Void MainForm::changeCameraRotation() {
	mainScene->setCameraRotation(camRotPitchBar->Value, camRotYawBar->Value, 0.0f);
	renderScene();
}

System::Void MainForm::camRotPitchBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeCameraRotation();
}

System::Void MainForm::camRotYawBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	changeCameraRotation();
}

// Various buttons:

System::Void MainForm::nextObjButton_Click(System::Object^  sender, System::EventArgs^  e) {
	mainScene->selectNextObject();
	checkButtons();
	updateObjectParams();
	renderScene();
}

System::Void MainForm::prevObjButton_Click(System::Object^  sender, System::EventArgs^  e) {
	mainScene->selectPreviousObject();
	checkButtons();
	updateObjectParams();
	renderScene();
}

System::Void MainForm::deleteObjButton_Click(System::Object^  sender, System::EventArgs^  e) {
	mainScene->deleteObject();
	checkButtons();
	updateObjectParams();
	renderScene();
}

System::Void MainForm::resetObjButton_Click(System::Object^  sender, System::EventArgs^  e) {
	mainScene->resetObject();
	updateObjectParams();
	renderScene();
}

System::Void MainForm::resetCamButton_Click(System::Object^  sender, System::EventArgs^  e) {
	mainScene->resetCamera();
	updateCameraParams();
	renderScene();
}

// Object reflection checkboxes:

System::Void MainForm::changeObjectReflection() {
	if (!isSettingParams) {
		mainScene->setObjectReflection(objReflectionXYCheckbox->Checked, objReflectionXZCheckbox->Checked, objReflectionYZCheckbox->Checked);
		renderScene();
	}
}

System::Void MainForm::objReflectionXYCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	changeObjectReflection();
}

System::Void MainForm::objReflectionXZCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	changeObjectReflection();
}

System::Void MainForm::objReflectionYZCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	changeObjectReflection();
}

// Color pickers:

System::Void MainForm::bgColorButton_Click(System::Object^  sender, System::EventArgs^  e) {
	ColorDialog ^dialog = gcnew ColorDialog();
	dialog->ShowHelp = true;
	dialog->Color = renderer->getBGColor();
	// Update the text box color if the user clicks OK 
	if (dialog->ShowDialog(this) == ::DialogResult::OK) {
		renderer->setBGColor(dialog->Color);
		bgColorButton->BackColor = dialog->Color;
		renderScene();
	}
}

System::Void MainForm::wfColorButton_Click(System::Object^  sender, System::EventArgs^  e) {
	ColorDialog ^dialog = gcnew ColorDialog();
	dialog->ShowHelp = true;
	dialog->Color = renderer->getWFColor();
	// Update the text box color if the user clicks OK 
	if (dialog->ShowDialog(this) == ::DialogResult::OK) {
		renderer->setWFColor(dialog->Color);
		wfColorButton->BackColor = dialog->Color;
		renderScene();
	}
}

System::Void MainForm::selectedColorButton_Click(System::Object^  sender, System::EventArgs^  e) {
	ColorDialog ^dialog = gcnew ColorDialog();
	dialog->ShowHelp = true;
	dialog->Color = renderer->getSelectedColor();
	// Update the text box color if the user clicks OK 
	if (dialog->ShowDialog(this) == ::DialogResult::OK) {
		renderer->setSelectedColor(dialog->Color);
		selectedColorButton->BackColor = dialog->Color;
		renderScene();
	}
}

// Projection radio buttons:

System::Void MainForm::changeProjectionMode() {
	mainScene->setProjectionMode(perspectiveButton->Checked);
	renderScene();
}

System::Void KDZ::MainForm::perspectiveButton_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e) {
	changeProjectionMode();
}

System::Void KDZ::MainForm::orthoButton_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e) {
	changeProjectionMode();
}

// Drawing mode radio buttons:

System::Void MainForm::changeDrawingMode() {
	if (wfRadioButton->Checked) mainScene->setDrawingMode(true, false);
	else if (solidRadioButton->Checked) mainScene->setDrawingMode(false, true);
	else if (bothRadioButton->Checked) mainScene->setDrawingMode(true, true);
	renderScene();
}

System::Void MainForm::wfRadioButton_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e) {
	changeDrawingMode();
}

System::Void MainForm::solidRadioButton_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e) {
	changeDrawingMode();
}

System::Void MainForm::bothRadioButton_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e) {
	changeDrawingMode();
}

// Culling:

System::Void MainForm::changeCulling() {
	if (cullOnRadioButton->Checked) mainScene->setCulling(true);
	else if (cullOffRadioButton->Checked) mainScene->setCulling(false);
	renderScene();
}

System::Void MainForm::cullOnRadioButton_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e) {
	changeCulling();
}

System::Void MainForm::cullOffRadioButton_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e) {
	changeCulling();
}
