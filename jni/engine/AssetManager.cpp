/*
 * AssetManager.cpp
 *
 *  Класс для работы с файлами из Java-среды.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#include "AssetManager.h"

AssetFile::AssetFile(AAssetManager* mgr, const char* fileName)
{
	this->asset = AAssetManager_open(mgr, fileName, AASSET_MODE_UNKNOWN);
}

AssetFile::~AssetFile()
{
	AAsset_close(this->asset);
}

int AssetFile::Read(void* buf, int size)
{
	return AAsset_read(this->asset, buf, size);
}

off_t AssetFile::Seek(off_t offset, int origin)
{
	return AAsset_seek(this->asset, offset, origin);
}

off_t AssetFile::Position()
{
	return AAsset_seek(this->asset, 0, SEEK_CUR);
}

off_t AssetFile::Size()
{
	return AAsset_getLength(this->asset);
}

AssetManager::AssetManager(AAssetManager* mgr)
{
	this->mgr = mgr;
}

AssetManager::AssetManager(JNIEnv* env, jobject activity)
{
	jclass c = env->GetObjectClass(activity);
	// получение метода getAssets из Activity
	jmethodID methodID = env->GetMethodID(c, "getAssets", "()Landroid/content/res/AssetManager;");
	this->mgr = AAssetManager_fromJava(env, env->CallObjectMethod(activity, methodID));
}

AssetFile* AssetManager::Open(const char* fileName)
{
	return new AssetFile(mgr, fileName);
}




