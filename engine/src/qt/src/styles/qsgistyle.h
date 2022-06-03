/****************************************************************************
** $Id: qt/qsgistyle.h   3.3.8   edited Jan 11 14:46 $
**
** Definition of SGI-like style class
**
** Created : 981231
**
** Copyright (C) 1998-2007 Trolltech ASA.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech ASA of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QSGISTYLE_H
#define QSGISTYLE_H

#ifndef QT_H
#include "qmotifstyle.h"
#include "qguardedptr.h"
#include "qwidget.h"
#endif // QT_H

#if !defined(QT_NO_STYLE_SGI) || defined(QT_PLUGIN)

#if defined(QT_PLUGIN)
#define Q_EXPORT_STYLE_SGI
#else
#define Q_EXPORT_STYLE_SGI Q_EXPORT
#endif

class QSGIStylePrivate;

class Q_EXPORT_STYLE_SGI QSGIStyle: public QMotifStyle
{
    Q_OBJECT
public:
    QSGIStyle( bool useHighlightCols = FALSE );
    virtual ~QSGIStyle();

#if !defined(Q_NO_USING_KEYWORD)
    using QMotifStyle::polish;
#endif
    void polish( QWidget* );
    void unPolish( QWidget* );
    void polish( QApplication* );
    void unPolish( QApplication* );

    void drawPrimitive( PrimitiveElement pe,
			QPainter *p,
			const QRect &r,
			const QColorGroup &cg,
			SFlags flags = Style_Default,
			const QStyleOption& = QStyleOption::Default ) const;

    void drawControl( ControlElement element,
		      QPainter *p,
		      const QWidget *widget,
		      const QRect &r,
		      const QColorGroup &cg,
		      SFlags how = Style_Default,
		      const QStyleOption& = QStyleOption::Default ) const;

    void drawComplexControl( ComplexControl control,
			     QPainter *p,
			     const QWidget* widget,
			     const QRect& r,
			     const QColorGroup& cg,
			     SFlags how = Style_Default,
#ifdef Q_QDOC
			     SCFlags sub = SC_All,
#else
			     SCFlags sub = (uint)SC_All,
#endif
			     SCFlags subActive = SC_None,
			     const QStyleOption& = QStyleOption::Default ) const;

    int pixelMetric( PixelMetric metric, const QWidget *widget = 0 ) const;

    QSize sizeFromContents( ContentsType contents,
			    const QWidget *widget,
			    const QSize &contentsSize,
			    const QStyleOption& = QStyleOption::Default ) const;

    QRect subRect( SubRect r, const QWidget *widget ) const;
    QRect querySubControlMetrics( ComplexControl control,
				  const QWidget *widget,
				  SubControl sc,
				  const QStyleOption& = QStyleOption::Default ) const;

protected:
    bool eventFilter( QObject*, QEvent*);

private:
    QSGIStylePrivate *d;

    uint isApplicationStyle :1;
#if defined(Q_DISABLE_COPY)
    QSGIStyle( const QSGIStyle & );
    QSGIStyle& operator=( const QSGIStyle & );
#endif

};

#endif // QT_NO_STYLE_SGI

#endif // QSGISTYLE_H
