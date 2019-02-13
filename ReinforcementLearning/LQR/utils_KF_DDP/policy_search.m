classdef policy_search
    properties
        H
        model
        idx
        dt
        nominal_state
        x
        Q
        R
    end

    methods
      
        function obj = policy_search(H, model, idx, dt, nominal_state, x, Q, R)
            obj.H = H;
            obj.model = model;
            obj.idx = idx;
            obj.dt = dt;
            obj.nominal_state = nominal_state;
            obj.x = x;
            obj.Q = Q;
            obj.R = R;
        end

        function loss = linear_search(obj, W)
            
            % for t=1:obj.H
            %     % control law:
            %     dx = compute_dx(obj.nominal_state(:, t), obj.x(:,t));
            %     delta_u = W * dx;
            %     % simulate:
            %     obj.x(:,t+1) = f_heli(obj.x(:,t), delta_u, obj.dt, obj.model, obj.idx);
            % end

            % loss = norm(obj.x(obj.idx.ned,:) - obj.nominal_state(obj.idx.ned,:));

            l = 0.0;
            diff = 0.0;
            for t=1:obj.H
                % control law:
                dx = compute_dx(obj.nominal_state, obj.x);
                delta_u = W(:, 1:21) * dx + W(:, 22);
                delta_u
                % simulate:
                noise_F_T = randn(6,1)*1;
                obj.x = f_heli(obj.x, delta_u, obj.dt, obj.model, obj.idx, noise_F_T);
                
                l = l + obj.x' * obj.Q * obj.x + delta_u' * obj.R * delta_u;
                diff = diff + norm(obj.x(obj.idx.ned) - obj.nominal_state(obj.idx.ned))^2;
                %obj.x' * obj.Q * obj.x
                %loss = loss + norm(obj.x(obj.idx.ned) - obj.nominal_state(obj.idx.ned))^2 ;
            end
            loss = l + sqrt(diff);
        end

        function [traininfo, trainedNet] = nn_search(obj, W0)

            % initialize input
            input(:, 1, 1, :) = obj.nominal_state;
            output(:, 1, 1, :) = obj.nominal_state;
            for t=1:obj.H-1
                dx = compute_dx(obj.nominal_state, input(:, 1, 1, t));
                delta_u = W0 * dx;
                input(:, 1, 1, t+1) = f_heli(input(:, 1, 1, t), delta_u, obj.dt, obj.model, obj.idx);
                output(:, 1, 1, t+1) = obj.nominal_state;
            end


            layers = [  fullyConnectedLayer(16)
                        reluLayer
                        fullyConnectedLayer(4)
                        customedRegressionLayer(input, obj.H, obj.dt, obj.idx, obj.model)]
                    
            options = trainingOptions('sgdm', ...
                                    'LearnRateDropFactor',0.2, ...
                                    'LearnRateDropPeriod',5, ...
                                    'MaxEpochs',20, ...
                                    'MiniBatchSize',128, ...
                                    'Plots','training-progress',...
                                    'CheckpointPath','/home/chen/Documents/playground/ReinforcementLearning/LQR/models/');
                                
            [traininfo, trainedNet] = trainNetwork(input, output, layers, options);
            
        end
    end

end