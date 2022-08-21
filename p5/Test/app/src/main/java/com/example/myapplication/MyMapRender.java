package com.example.myapplication;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;

public class MyMapRender extends SurfaceView {
    private static final int SHOP_RADIUS = 5;

    private CVector shops;
    //[[x,y],[x,y]....]
    private CVector mapPoints;

    //[[a,b,w],[a,b,w]...]
    private CVector mapEdges;
    private CVector path;
    private Point mapOffset;
    private Paint shopPaint;
    private Paint shopNamePaint;
    private Paint roadPaint;
    private Paint roadDistancePaint;
    private Paint pathPaint;
    private boolean showCenter = true;

    private Bitmap centerBmp;

    public MyMapRender(Context context) {
        super(context);
        init();
    }

    public MyMapRender(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public MyMapRender(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    public MyMapRender(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init();
    }

    private void init() {
        centerBmp = BitmapFactory.decodeResource(getResources(), R.drawable.loc);
        mapOffset = new Point();

        shopPaint = new Paint();
        shopPaint.setColor(Color.BLUE);

        pathPaint = new Paint();
        pathPaint.setColor(Color.RED);

        shopNamePaint = new Paint();
        shopNamePaint.setTextSize(25);
        shopNamePaint.setColor(Color.BLUE);

        roadPaint = new Paint();
        roadPaint.setColor(Color.DKGRAY);
        roadDistancePaint = new Paint();
        roadDistancePaint.setColor(Color.BLACK);
        setOnTouchListener(new OnTouchListener() {
            int x = 0;
            int y = 0;

            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        x = (int) motionEvent.getX();
                        y = (int) motionEvent.getY();
                        break;
                    case MotionEvent.ACTION_MOVE:
                        mapOffset.offset((int) motionEvent.getX() - x, (int) motionEvent.getY() - y);
                        x = (int) motionEvent.getX();
                        y = (int) motionEvent.getY();
                        invalidate();
                        break;
                }
                return true;
            }
        });
    }

    private void drawPath(Canvas canvas) {
        if (path == null || path.size() == 0) return;
        for (int i = 0; i < path.size() - 1; i++) {
            long edgePtr = path.getInt(i);
            int a = path.getInt(i);
            long aPtr = mapPoints.get(a);

            int b = path.getInt(i + 1);
            long bPtr = mapPoints.get(b);

            int x1 = CVector.intAt(aPtr, 0) + mapOffset.x;
            int y1 = CVector.intAt(aPtr, 1) + mapOffset.y;

            int x2 = CVector.intAt(bPtr, 0) + mapOffset.x;
            int y2 = CVector.intAt(bPtr, 1) + mapOffset.y;

            canvas.drawLine(x1, y1, x2, y2, pathPaint);
            canvas.drawRect(x1 - 5, y1 - 5, x1 + 5, y1 + 5, pathPaint);
            canvas.drawRect(x2 - 5, y2 - 5, x2 + 5, y2 + 5, pathPaint);
//            canvas.drawText(w + "", (x1 + x2) / 2, (y1 + y2) / 2, roadDistancePaint);
        }
    }

    private void drawRoads(Canvas canvas) {

        if (mapEdges == null || mapEdges.size() == 0) return;
        for (int i = 0; i < mapEdges.size(); i++) {
            long edgePtr = mapEdges.getInt(i);
            int a = CVector.intAt(edgePtr, 0);
            int b = CVector.intAt(edgePtr, 1);
            int w = CVector.intAt(edgePtr, 2);

            long aPtr = mapPoints.get(a);
            long bPtr = mapPoints.get(b);

            int x1 = CVector.intAt(aPtr, 0) + mapOffset.x;
            int y1 = CVector.intAt(aPtr, 1) + mapOffset.y;

            int x2 = CVector.intAt(bPtr, 0) + mapOffset.x;
            int y2 = CVector.intAt(bPtr, 1) + mapOffset.y;

            canvas.drawLine(x1, y1, x2, y2, roadPaint);
            canvas.drawRect(x1 - 1, y1 - 1, x1 + 1, y1 + 1, roadPaint);
            canvas.drawRect(x2 - 1, y2 - 1, x2 + 1, y2 + 1, roadPaint);
            canvas.drawText(w + "", (x1 + x2) / 2, (y1 + y2) / 2, roadDistancePaint);
        }
    }

    private void drawShops(Canvas canvas) {
        if (shops == null || shops.size() == 0) return;

        for (int i = 0; i < shops.size(); i++) {
            long shopPtr = shops.get(i);
            int x = Shop.getX(shopPtr) + mapOffset.x;
            int y = Shop.getY(shopPtr) + mapOffset.y;
            String name = Shop.getName(shopPtr);

            canvas.drawOval(x - SHOP_RADIUS, y - SHOP_RADIUS, x + SHOP_RADIUS, y + SHOP_RADIUS, shopPaint);
            Rect textBound = new Rect();
            shopNamePaint.getTextBounds(name, 0, name.length() - 1, textBound);
            textBound.offset(x, y - SHOP_RADIUS);
            canvas.drawText(name, textBound.left - (textBound.width() / 2), textBound.top, shopNamePaint);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        drawRoads(canvas);
        drawShops(canvas);
        drawPath(canvas);
        if (showCenter) {
            Point logicCenter = getCenter();
            Point center = new Point(getWidth() / 2, getHeight() / 2);
            canvas.drawBitmap(centerBmp,
                    new Rect(0, 0, centerBmp.getWidth(), centerBmp.getHeight()),
                    new Rect(center.x - 20, center.y - 40, center.x + 20, center.y),
                    shopNamePaint);
            canvas.drawText(String.format("center: (%d,%d)", logicCenter.x, logicCenter.y), 0, 50, shopNamePaint);
        }
    }

    public Point getCenter() {
        return new Point(getWidth() / 2 - mapOffset.x, getHeight() / 2 - mapOffset.y);
    }

    public CVector getShops() {
        return shops;
    }

    public void setShops(CVector shops) {
        this.shops = shops;
        invalidate();
    }

    public Point getMapOffset() {
        return mapOffset;
    }

    public void offset(int dx, int dy) {
        this.mapOffset.offset(dx, dy);
        invalidate();
    }

    public void setOffset(int x, int y) {
        this.mapOffset.x = x;
        this.mapOffset.y = y;
        invalidate();
    }

    public Point getOffset() {
        return mapOffset;
    }

    public boolean isShowCenter() {
        return showCenter;
    }

    public void setShowCenter(boolean showCenter) {
        this.showCenter = showCenter;
        invalidate();
    }

    public CVector getPath() {
        return path;
    }

    public void setPath(CVector path) {
        if (this.path != null && this.path.getObjId() != 0) {
            this.path.finalize();
        }
        this.path = path;
        invalidate();
    }

    public CVector getMapPoints() {
        return mapPoints;
    }

    public void setMapPoints(CVector mapPoints) {
        this.mapPoints = mapPoints;
    }

    public CVector getMapEdges() {
        return mapEdges;
    }

    public void setMapEdges(CVector mapEdges) {
        this.mapEdges = mapEdges;
    }
}
