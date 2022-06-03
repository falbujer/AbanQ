@AQOdsChart(const QString& name, AQOdsLength* width, AQOdsLength* height);
@AQOdsChart(const QString& name, AQOdsCentimeters* width, AQOdsCentimeters* height);
void addRange(AQOdsCellRange* range);
void addSeries(AQOdsSeries* series);
void setTitle(const QString& title);
void setSubtitle(const QString& subtitle);
void setXAxisLabel(const QString& x_axis_label);
void setYAxisLabel(const QString& y_axis_label);
