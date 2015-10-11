#include "SettingsConverter.h"
#include <io\BinaryWriter.h>
#include <utils\PlainTextReader.h>
/*
void SettingsLoader::convert(JSONReader& reader, BinaryWriter& writer) {
	Category* root = reader.getCategory("settings");
	if (root != 0) {
		GameSettings definitions;
		m_SettingsTranslator.read(root, &definitions);
		m_SettingsTranslator.saveChunk(writer, 1000, &definitions);
	}
}

void SettingsLoader::load(BinaryLoader* loader) {
	while (loader->openChunk() == 0) {
		if (loader->getChunkID() == 1000) {
			m_SettingsTranslator.readChunk(*loader, &m_Settings);
		}
		loader->closeChunk();
	}
}
*/

