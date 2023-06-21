#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"

class Plane
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetCamera(Camera* camera) { Plane::camera = camera; }
	static void SetInput(Input* input) { Plane::input = input; }
	static void SetDXInput(DXInput* dxInput) { Plane::dxInput = dxInput; }

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//�ړ�
	void Move();

	//�Z�b�^�[
	void SetObject(FbxObject3D* object);
	void SetSRV(ID3D12DescriptorHeap* SRV);

	//�Q�b�^�[
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }

	//�ÓI�����o�ϐ�
private:
	//�J����
	static Camera* camera;
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;

	//�����o�ϐ�
public:

	//�I�u�W�F�N�g
	std::list<std::unique_ptr<FbxObject3D>>object;

	//���W
	XMFLOAT3 position = {0.0f,0.0f,0.0f};
	//��]
	XMFLOAT3 rotation = {0.0f,0.0f,0.0f};
	//�T�C�Y
	XMFLOAT3 scale = {1.0f,1.0f,1.0f};
};

