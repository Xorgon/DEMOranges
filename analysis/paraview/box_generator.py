# create a new 'Plane'
plane1 = Plane()

# Properties modified on plane1
plane1.Origin = [-0.5, -0.5, -0.5]
plane1.Point1 = [-0.5, -0.5, 0.5]
plane1.Point2 = [0.5, -0.5, -0.5]

# create a new 'Plane'
plane2 = Plane()

# Properties modified on plane2
plane2.Origin = [-0.5, 0.5, -0.5]
plane2.Point1 = [-0.5, 0.5, 0.5]
plane2.Point2 = [0.5, 0.5, -0.5]

# create a new 'Plane'
plane3 = Plane()

# Properties modified on plane3
plane3.Origin = [-0.5, -0.5, -0.5]
plane3.Point1 = [-0.5, -0.5, 0.5]
plane3.Point2 = [-0.5, 0.5, -0.5]

# create a new 'Plane'
plane4 = Plane()

# Properties modified on plane4
plane4.Origin = [0.5, -0.5, -0.5]
plane4.Point1 = [0.5, -0.5, 0.5]
plane4.Point2 = [0.5, 0.5, -0.5]

# create a new 'Plane'
plane5 = Plane()

# Properties modified on plane5
plane5.Origin = [-0.5, -0.5, -0.5]
plane5.Point1 = [0.5, -0.5, -0.5]
plane5.Point2 = [-0.5, 0.5, -0.5]

# create a new 'Plane'
plane6 = Plane()

# Properties modified on plane6
plane6.Origin = [-0.5, -0.5, 0.5]
plane6.Point1 = [0.5, -0.5, 0.5]
plane6.Point2 = [-0.5, 0.5, 0.5]

renderView1 = GetActiveViewOrCreate('RenderView')
plane1Display = Show(plane1, renderView1)
plane2Display = Show(plane2, renderView1)
plane3Display = Show(plane3, renderView1)
plane4Display = Show(plane4, renderView1)
plane5Display = Show(plane5, renderView1)
plane6Display = Show(plane6, renderView1)

# Properties modified on plane6Display
plane6Display.Opacity = 0.5

# Properties modified on plane4Display
plane4Display.Opacity = 0.5

# Properties modified on plane2Display
plane2Display.Opacity = 0.5

box_length = input("box_length = ")

# Properties modified on plane1Display
plane1Display.Scale = [box_length, box_length, box_length]
plane1Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane1Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane2Display
plane2Display.Scale = [box_length, box_length, box_length]
plane2Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane2Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane3Display
plane3Display.Scale = [box_length, box_length, box_length]
plane3Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane3Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane4Display
plane4Display.Scale = [box_length, box_length, box_length]
plane4Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane4Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane5Display
plane5Display.Scale = [box_length, box_length, box_length]
plane5Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane5Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane6Display
plane6Display.Scale = [box_length, box_length, box_length]
plane6Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane6Display.PolarAxes.Scale = [box_length, box_length, box_length]