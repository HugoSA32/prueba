#ifndef _PDFBRIDGEEVINCE_H_
#define _PDFBRIDGEEVINCE_H_

#include "PDFBridge.h"

#include <string>

#include <gio/gio.h>


class PDFBridgeEvince : public PDFBridge {
private:
	GDBusConnection* dbusConnection = nullptr;
	GDBusProxy *windowProxy = nullptr;
	GDBusProxy *daemonProxy = nullptr;

	std::string selection = "";

	static void OnSignal(GDBusProxy *proxy, gchar *senderName, gchar *signalName, GVariant *parameters, gpointer userData);
public:
	PDFBridgeEvince();
	~PDFBridgeEvince();

	void OpenDocument(const filesystem::path &pdfPath);
	void CloseDocument();
	void DocumentSearch(const std::string &str, bool wholeWordsOnly, bool caseSensitive);
	bool HasNewSelection();
	std::string GetSelection() const;
};

#endif//_PDFBRIDGEEVINCE_H_
