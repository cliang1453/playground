classdef customedRegressionLayer < nnet.cnn.layer.RegressionOutputLayer
        
    properties
        % (Optional) Layer properties.

        % Layer properties go here.
        x;
        x_nominal;
        H;
        dt;
        model;
        idx;
        Q;
        R;
        pred_x;
        A;
        B;
    end
 
    methods
        function layer = customedRegressionLayer(internalLayer, x, H, dt, model, idx, A, B, Q, R, x_nominal)           
            % Layer constructor function goes here.
            layer@nnet.cnn.layer.RegressionOutputLayer(internalLayer);
            layer.x = x;
            layer.x_nominal = x_nominal;
            layer.dt = dt; 
            layer.H = H;
            layer.model = model;
            layer.idx = idx;
            layer.A = A;
            layer.B = B;
            layer.Q = Q; 
            layer.R = R;
        end

        function loss = forwardLoss(layer, delta_u, Y)
            %   Inputs:
            %       layer - Output layer
            %       predicted control - delta_u
            %       nominal trajectory - x_star
            %   Output:
            %       loss  - Loss between current trajectory and norminal
            %       trajectory
            
            layer.pred_x = zeros(21, layer.H);
            layer.pred_x(:, 1) = reshape(layer.x(:, :, :, 1), [21, 1]);

            for i = 1:layer.H-1
                curr_x = reshape(layer.x(:, :, :, i), [21, 1]);
                curr_delta_u = reshape(delta_u(:, :, :, i), [4, 1]);
                layer.pred_x(:, i+1) = f_heli(curr_x, curr_delta_u, layer.dt, layer.model, layer.idx);
            end
            
            loss = (layer.pred_x - layer.x_star).' * layer.Q * (layer.pred_x - x_star) + delta_u.' * layer.R * delta_u;

        end
        
        function dLdY = backwardLoss(layer, delta_u, Y)
            
            % Backward propagate the derivative of the loss function.
            % Input: 
            %       layer - Output layer
            %       predicted control - delta_u
            %       norminal trajectory - x_star
            
            % Assume the transition function is linear in both x and
            % u (i.e. x_{t+1} = T(x_t, u_t) = Ax_t + Bu_t + C)
            dLdY = 2 * (layer.A * (layer.pred_x - layer.x_star)).' * layer.Q * layer.B ...
                + 2 * (layer.B * delta_u).' * layer.R * layer.B;
            
            % The gradient dimension: [layer.H * 4]
        end
    end
end