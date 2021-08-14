struct ImageMatchResult
{
	int x = 0;
	int y = 0;
	double score = 0;
	int width = 0;
	int height = 0;

	operator RRectUIElement()
	{
		RRectUIElement uiElement(*this);
		return uiElement;
	}
};